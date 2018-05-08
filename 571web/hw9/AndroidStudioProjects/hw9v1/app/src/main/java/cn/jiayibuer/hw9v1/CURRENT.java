package cn.jiayibuer.hw9v1;

import android.app.DownloadManager;
import android.app.Fragment;
import android.app.LoaderManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.IntentSender;
import android.content.Loader;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.database.Cursor;
import android.database.DatabaseErrorHandler;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Picture;
import android.graphics.drawable.Drawable;
import android.media.Image;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;

import android.os.Debug;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.RecoverySystem;
import android.os.UserHandle;
import android.provider.ContactsContract;
import android.provider.MediaStore;
import android.service.quicksettings.Tile;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.widget.ContentLoadingProgressBar;
import android.text.Html;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.webkit.DownloadListener;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.SimpleAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;

import com.facebook.FacebookSdk;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.*;
import com.facebook.login.LoginBehavior;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareContent;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.LikeView;
import com.facebook.share.widget.ShareButton;
import com.facebook.share.widget.ShareDialog;


public class CURRENT extends android.support.v4.app.Fragment {


    //{
    // "sts":"abc",
    // "lastprice":"81.2300",
    // "change":"1.77(2.23%)",
    // "timestamp":"2017-11-24",
    // "open":"81.9000",
    // "close":"81.2300",
    // "daysrange":"80.2200-81.9800",
    // "volume":"701,755",
    // "image":"http:\/\/cs-server.usc.edu:45678\/hw\/hw8\/images\/Up.png"
    // }
    StockJY jy;
    // basic info
    private String cr_symbol;
    private Boolean from_favorite;
    private View view;

    private SimpleAdapter stockInfoAdapter;
    //for progress bar
    private ProgressBar progressBar;
    private int progressStatus = 0;
    private Handler handler;

    //for listview
    private ListView listView;
    private String[] title = {"Stock Symbol","Last Price","Change","TimeStamp","Open","close","Day's Range","Volume"};
    private String[] index = {"sts"         ,"lastprice" ,"change","timestamp","open","close","daysrange"  ,"volume"};
    private  ArrayList<HashMap<String, Object>> arrayList = new ArrayList<>();

    //for graph
    private String indicators[]={"Price","SMA","EMA","MACD","RSI","ADX","CCI","STOCH","BBANDS"};
    private String record;
    private int tag=0;
    private Button indicator;
    private WebView webview;
    private ProgressBar webLoading;
    private String cr_url="http://stock4jyr.us-west-1.elasticbeanstalk.com/phplink/pricegraph.php";
    private Spinner spin;
    private ArrayAdapter<String> sp_adapter;
    //for favorite
    private ImageButton favorite;
    private int fav_flag;
    private  ArrayList<HashMap<String, Object>> favoriteList = new ArrayList<HashMap<String, Object>>();
    private HashMap<String, Object> map = new HashMap<>();
//fb
    private CallbackManager callbackManager;
    private ShareDialog shareDialog;
    private static String TAG = SharePhotoContent.class.getName();



    private String crt_url;

//    private ShareButton facebookBtn;
    private ImageButton facebookBtn;
    private ShareLinkContent linkContent;

    public CURRENT() {
        // Required empty public constructor
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        HashMap mp=((show_stock) context).getTitles();
        cr_symbol = (String)mp.get("symbol");
        from_favorite = (Boolean)mp.get("from_favorite");
        if (mp.get("favoriteList") != null) {
            favoriteList =(ArrayList<HashMap<String,Object>>)mp.get("favoriteList") ;
            System.out.println("symbol::::"+favoriteList);

        }
       Interface_callback = (callbackValue)context;

    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


       // webLoading = getActivity().findViewById(R.id.progressBar3);

        FacebookSdk.getApplicationContext();
        callbackManager = CallbackManager.Factory.create();
        //initial share dialog
        shareDialog = new ShareDialog(this);
        shareDialog.registerCallback(callbackManager, callback);

        crt_url=cr_url;


        stockInfoAdapter = new SimpleAdapter(
                getContext(),
                arrayList,
                R.layout.stock_list,
                new String[] {"Title", "Value"},
                new int[] {R.id.ItemTitle,R.id.ItemValue}
        );
        update();

        //spinner
        sp_adapter = new ArrayAdapter<>(getActivity(),android.R.layout.simple_list_item_1,indicators);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {


        view = inflater.inflate(R.layout.fragment_current, container, false);
        System.out.println("cr_symbol:"+cr_symbol);
        progressBar = view.findViewById(R.id.progressBar2);


        //progressBar.setVisibility(View.INVISIBLE);
        //listview
        listView = view.findViewById(R.id.current_main);
        //stockInfoAdapter = new StockInfoAdapter(arrayList);
//        View empty_view = getLayoutInflater().inflate(R.layout.empty,null);
//        listView.setEmptyView(empty_view.findViewById(R.id.empty));
//        handler.post(new Runnable() {
//            @Override
//            public void run() {
                listView.setAdapter(stockInfoAdapter);
//            }
//        });
        favorite = view.findViewById(R.id.favorite_btn);
        if(favoriteList!=null){
            for(int i=0;i<favoriteList.size();i++){
                System.out.println("favflag  "+cr_symbol+"favlist"+favoriteList.get(i).get("symbol"));
                if(cr_symbol.equals(favoriteList.get(i).get("symbol"))){
                    favorite.setImageResource(R.drawable.filled);
                    fav_flag=1;
                    break;
                }
                else{
                    favorite.setImageResource(R.drawable.empty);
                    fav_flag=0;
                }
            }
        }
        else{
            favorite.setImageResource(R.drawable.empty);
            fav_flag=0;
        }
        System.out.println("favflag  "+fav_flag);
        //for favorite

//        if(from_favorite == true){
//            favorite.setImageResource(R.drawable.filled);
//            fav_flag=1;
//        }
        favorite.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                try{
                    if(map!=null) {
                        if(fav_flag != 1){
                            favorite.setImageResource(R.drawable.filled);
                            favoriteList.add(map);
                            fav_flag = 1;
                        }
                        else if(fav_flag == 1){
                            favorite.setImageResource(R.drawable.empty);
                            favoriteList.remove(map);
                            fav_flag=0;
                        }
                    }
                    else{
                        view.setEnabled(false);
                    }
                }catch (Exception e){

                }

            }

        });
        Interface_callback.sendDataOfRecord(favoriteList);

        //for facebook

        //ShareButton
        facebookBtn=view.findViewById(R.id.facebook_btn);
        //facebookBtn.setShareContent(linkContent);
        facebookBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (ShareDialog.canShow(ShareLinkContent.class)) {
            linkContent = new ShareLinkContent.Builder()
                    .setContentUrl(Uri.parse("http://export.highcharts.com/"))
                    .build();
            shareDialog.show(linkContent);
        }
//                   Bitmap pic = getScreen();
//                    SharePhoto photo = new SharePhoto.Builder()
//                            .setBitmap(pic)
//                            .build();
////                    linkContent = new ShareLinkContent.Builder()
////                            .setContentUrl(Uri.parse(webview.getUrl()))
////                            .build();
//                    SharePhotoContent spcontent = new SharePhotoContent.Builder()
//                            .addPhoto(photo)
//                            .build();
//                    ShareDialog.show(getActivity(),spcontent);
               // }
            }
        });


        //click to show graph
        indicator = view.findViewById(R.id.change_ind);

        webLoading = view.findViewById(R.id.progressBar3);
        //for webview/graph
        webview = view.findViewById(R.id.webview_graph);
        //String cr_url="http://10.0.2.2/hw9/indicgraph.php";
        webview.setWebViewClient(new WebViewClient(){

            @Override
            public void onPageFinished(WebView view, String url) {
                super.onPageFinished(view, url);
                webLoading.setVisibility(View.GONE);
            }

            @Override
            public void onLoadResource(WebView view, String url) {
                super.onLoadResource(view, url);
                webLoading.setVisibility(View.VISIBLE);
            }
        });

        WebSettings webSettings= webview.getSettings();
        webSettings.setJavaScriptEnabled(true);
        webview.setDrawingCacheEnabled(true);
        //webSettings.setAppCacheEnabled(true);
        webview.setWebChromeClient(new WebChromeClient() {
            public void onProgressChanged(WebView view, int progress) {
                // Activities and WebViews measure progress with different scales.
                // The progress meter will automatically disappear when we reach 100%
               webLoading.setVisibility(View.VISIBLE);
            }

            @Override
            public void onShowCustomView(View view, CustomViewCallback callback) {
                super.onShowCustomView(view, callback);
                webLoading.setVisibility(View.GONE);
            }
        });


        //spinner
        //for spinner
        spin =view.findViewById(R.id.spinner_ind);
        spin.setAdapter(sp_adapter);
        spin.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                    switch (i) {
                        case 0:
                            record = "Price";
                            webview.addJavascriptInterface(new WebAppInterface(getActivity(),cr_symbol),"Android");
                            webview.loadUrl(cr_url);
                            indicator.setEnabled(false);
                            break;
                        case 1:
                            record = "SMA";
                            break;
                        case 2:
                            record = "EMA";
                            break;
                        case 3:
                            record = "MACD";
                            break;
                        case 4:
                            record = "RSI";
                            break;
                        case 5:
                            record = "ADX";
                            break;
                        case 6:
                            record = "CCI";
                            break;
                        case 7:
                            record = "STOCH";
                            break;
                        case 8:
                            record = "BBANDS";
                            break;
                    }

                if(i != tag) {
                    indicator.setEnabled(true);
                }
                else {
                    indicator.setEnabled(false);
                }
                tag = i;

            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        indicator.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(record=="price"){
                    webview.addJavascriptInterface(new WebAppInterface(getActivity(),cr_symbol),"Android");

                    webview.loadUrl(cr_url);

                    crt_url=cr_url;
                }
                else{
                    String ind_url="http://stock4jyr.us-west-1.elasticbeanstalk.com/phplink/indicgraph.php";
                    //String ind_url="http://10.0.2.2/hw9/phplink/indicgraph.php";
                    webview.addJavascriptInterface(new WebAppInterface(getActivity(),cr_symbol,record),"Android");
                    webview.loadUrl(ind_url);
                    crt_url=ind_url;
                }
                view.setEnabled(false);
            }

        });

        return view;
    }

    @Override
    public void onResume() {

        super.onResume();
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }


    @Override
    public void onActivityResult(final int requestCode, final int resultCode, final Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        callbackManager.onActivityResult(requestCode, resultCode, data);
    }

    private FacebookCallback<Sharer.Result> callback = new FacebookCallback<Sharer.Result>() {
        @Override
        public void onSuccess(Sharer.Result result) {
            Log.v(TAG, "Successfully posted");
            // Write some code to do some operations when you shared content successfully.
        }

        @Override
        public void onCancel() {
            Log.v(TAG, "Sharing cancelled");
            // Write some code to do some operations when you cancel sharing content.
        }

        @Override
        public void onError(FacebookException error) {
            Log.v(TAG, error.getMessage());
            // Write some code to do some operations when some error occurs while sharing content.
        }
    };


    //回调接口，用来和activity互通信息
    private callbackValue Interface_callback;
    public interface callbackValue {
        public void sendDataOfRecord(ArrayList<HashMap<String,Object>> data);
    }

    public Bitmap getScreen(){
        Bitmap bmp = Bitmap.createBitmap(webview.getWidth(),1,Bitmap.Config.ARGB_8888);
        int rowBytes = bmp.getRowBytes();
        bmp=null;
        if(rowBytes*webview.getHeight()>=getAvailMemory()){
            System.out.println("too big to draw");
            return null;
        }
        bmp=Bitmap.createBitmap(webview.getWidth(),webview.getHeight(),Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bmp);
        webview.draw(canvas);
        System.out.println("successful drawing pics: height"+webview.getHeight());
    return bmp;
    }

    private long getAvailMemory(){
    return  Runtime.getRuntime().maxMemory();
    }

    private void update(){
        UpdateTextTask updateTextTask = new UpdateTextTask(getContext());
        updateTextTask.execute();
    }

    class UpdateTextTask extends AsyncTask<Void,Integer,Integer> {
        private Context context;
        UpdateTextTask(Context context) {
            this.context = context;
        }

        /**
         * 运行在UI线程中，在调用doInBackground()之前执行
         */
        @Override
        protected void onPreExecute() {
            Toast.makeText(context,"begin executing",Toast.LENGTH_SHORT).show();
        }
        /**
         * 后台运行的方法，可以运行非UI线程，可以执行耗时的方法
         */
        @Override
        protected Integer doInBackground(Void... params) {

            RequestQueue requestQueue = Volley.newRequestQueue(getContext());
            //https://jsonplaceholder.typicode.com/users
            //http://jiayiren-stock.us-west-1.elasticbeanstalk.com/stockinfo.php?symbol=%22a%22
//        String url = "http://jiayiren-stock.us-west-1.elasticbeanstalk.com/stockinfo.php?symbol=%22"+symbol+"%22";
            String url = "http://stock4jyr.us-west-1.elasticbeanstalk.com/phplink/data/stockinfo.php?symbol=%22" + cr_symbol + "%22";
            JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, url, null,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            try {

//                            String sts = response.getString("sts");
                                String sts2 = response.getString(index[0]);
                                Toast.makeText(getContext(), sts2, Toast.LENGTH_LONG).show();

                                System.out.println("index_length" + index.length);
                                for (int i = 0; i < index.length; i++) {
                                    HashMap<String, Object> m = new HashMap<>();
                                    String index_val = response.getString(index[i]);
//                                    if(title[i].equals("Change")){
//                                        m.put("Title", title[i]);
//                                        if(index_val.substring(index_val.indexOf("(")+1).equals("-")){
//                                        m.put("Value", index_val+ R.drawable.down);}
//                                        else if(! index_val.substring(index_val.indexOf("(")+1).equals("-")){
//                                            m.put("Value", index_val+R.drawable.up);}
//                                    }
//                                    else {
                                        m.put("Title", title[i]);
                                        m.put("Value", index_val);
                                    //}
                                    arrayList.add(m);
                                }
                                for(int i=0;i<3;i++){
                                    map.put("symbol",arrayList.get(0).get("Value"));
                                    map.put("price",arrayList.get(1).get("Value"));
                                    map.put("change",arrayList.get(2).get("Value"));
                                }



                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                            stockInfoAdapter.notifyDataSetChanged();
                        }
                    },
                    new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            Toast.makeText(getContext(), "Nothing found!", Toast.LENGTH_SHORT);

                        }
                    });
            requestQueue.add(jsonObjectRequest);


            return null;
        }

        /**
         * 运行在ui线程中，在doInBackground()执行完毕后执行
         */
        @Override
        protected void onPostExecute(Integer integer) {
            progressBar.setVisibility(View.GONE);
            Toast.makeText(context,"finish",Toast.LENGTH_SHORT).show();

        }

        /**
         * 在publishProgress()被调用以后执行，publishProgress()用于更新进度
         */
        @Override
        protected void onProgressUpdate(Integer... values) {
            progressBar.setVisibility(View.VISIBLE);
        }
    }
}




