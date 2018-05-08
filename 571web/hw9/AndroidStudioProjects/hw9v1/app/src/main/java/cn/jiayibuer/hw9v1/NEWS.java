package cn.jiayibuer.hw9v1;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
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


public class NEWS extends android.support.v4.app.Fragment {


    private View view;
    private String news_symbol;

    private ListView listView;
    private ArrayList<HashMap<String, Object>> listItem = new ArrayList<>();
    private String[] news = {"news1", "news2", "news3", "news4", "news5"};

    private SimpleAdapter listItemAdapter;
    private String news_url ;//= "http://stock4jyr.us-west-1.elasticbeanstalk.com/phplink/data/news.php?symbol=%22"++"%22";

    //https://jsonplaceholder.typicode.com/users
    //http://jiayiren-stock.us-west-1.elasticbeanstalk.com/stockinfo.php?symbol=%22a%22
//        String url = "http://jiayiren-stock.us-west-1.elasticbeanstalk.com/stockinfo.php?symbol=%22"+symbol+"%22";


    public NEWS() {
        // Required empty public constructor
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
//        if (context instanceof OnFragmentInteractionListener) {
//            mListener = (OnFragmentInteractionListener) context;
//        } else {
//            throw new RuntimeException(context.toString()
//                    + " must implement OnFragmentInteractionListener");
//        }
        HashMap map = ((show_stock) context).getTitles();
        news_symbol = (String) map.get("symbol");
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        news_url = "http://stock4jyr.us-west-1.elasticbeanstalk.com/phplink/data/news.php?symbol=%22"+news_symbol+"%22";

        listItemAdapter = new SimpleAdapter(
                getContext(),
                listItem,
                R.layout.news_list,
                new String[]{"newsTitle", "newsAuthor", "newsDate"},
                new int[]{R.id.newsTitle, R.id.newsAuthor, R.id.newsDate}
        );
        update();
//        RequestQueue requestQueue = Volley.newRequestQueue(getContext());
//        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, news_url, null,
//                new Response.Listener<JSONObject>() {
//                    @Override
//                    public void onResponse(JSONObject response) {
//                        try {
//                            String a;
//                            JSONArray sts2 = response.getJSONArray(news[0]);
//                            a = sts2.getJSONObject(0).getString("0");
//                            System.out.println("newsggggg" + a);
//                            Toast.makeText(getContext(), a, Toast.LENGTH_LONG).show();
//
//                            for (int i = 0; i < news.length; i++) {
//                                HashMap<String, Object> map = new HashMap<>();
//                                JSONArray index_val = response.getJSONArray(news[i]);
//                                String title = index_val.getJSONObject(0).getString("0");
//                                String author = index_val.getJSONObject(1).getString("0");
//                                String pubDate = index_val.getString(2);
//                                String link = index_val.getJSONObject(3).getString("0");
//                                map.put("newsTitle", title);
//                                map.put("newsAuthor", "author: " + author);
//                                map.put("newsDate", "Date: " + pubDate);
//                                map.put("link", link);
//                                listItem.add(map);
//                            }
//
//                        } catch (JSONException e) {
//                            e.printStackTrace();
//                        }
//                        listItemAdapter.notifyDataSetChanged();
//
//                    }
//                },
//                new Response.ErrorListener() {
//                    @Override
//                    public void onErrorResponse(VolleyError error) {
//                        Toast.makeText(getContext(), "Nothing found!", Toast.LENGTH_SHORT);
//
//                    }
//                });
//        requestQueue.add(jsonObjectRequest);

    }



    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view = inflater.inflate(R.layout.fragment_news, container, false);
        listView = view.findViewById(R.id.news_main);
        listView.setAdapter(listItemAdapter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {

                String cr_url=(String)listItem.get(i).get("link");
                Uri uri = Uri.parse(cr_url);
                Intent it = new Intent(Intent.ACTION_VIEW, uri);
                startActivity(it);


            }
        });


        return view;
    }



    @Override
    public void onDetach() {
        super.onDetach();
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
            Toast.makeText(context,"开始执行",Toast.LENGTH_SHORT).show();
        }
        /**
         * 后台运行的方法，可以运行非UI线程，可以执行耗时的方法
         */
        @Override
        protected Integer doInBackground(Void... params) {

                    RequestQueue requestQueue = Volley.newRequestQueue(getContext());
                    JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, news_url, null,
                            new Response.Listener<JSONObject>() {
                                @Override
                                public void onResponse(JSONObject response) {
                                    try {
                                        String a;
                                        JSONArray sts2 = response.getJSONArray(news[0]);
                                        a = sts2.getJSONObject(0).getString("0");
                                        System.out.println("newsggggg" + a);
                                        Toast.makeText(getContext(), a, Toast.LENGTH_LONG).show();

                                        for (int i = 0; i < news.length; i++) {
                                            HashMap<String, Object> map = new HashMap<>();
                                            JSONArray index_val = response.getJSONArray(news[i]);
                                            String title = index_val.getJSONObject(0).getString("0");
                                            String author = index_val.getJSONObject(1).getString("0");
                                            String pubDate = index_val.getString(2);
                                            String link = index_val.getJSONObject(3).getString("0");
                                            map.put("newsTitle", title);
                                            map.put("newsAuthor", "author: " + author);
                                            map.put("newsDate", "Date: " + pubDate);
                                            map.put("link", link);
                                            listItem.add(map);
                                        }

                                    } catch (JSONException e) {
                                        e.printStackTrace();
                                    }
                                    listItemAdapter.notifyDataSetChanged();

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
            Toast.makeText(context,"执行完毕",Toast.LENGTH_SHORT).show();
        }

        /**
         * 在publishProgress()被调用以后执行，publishProgress()用于更新进度
         */
        @Override
        protected void onProgressUpdate(Integer... values) {
            view.findViewById(R.id.progressBar).setVisibility(View.VISIBLE);
        }
    }
}

