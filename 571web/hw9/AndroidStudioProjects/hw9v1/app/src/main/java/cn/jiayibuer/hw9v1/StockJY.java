package cn.jiayibuer.hw9v1;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.ContextMenu;
import android.view.MenuInflater;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Spinner;
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

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;


public class StockJY extends AppCompatActivity {

    private String url;
    private ArrayList<HashMap<String, Object>> VALUE =new ArrayList<>();
    private String stockSymbol;

    private Button search;
    private AutoCompleteTextView symbolText;
    private Button clear;
    private ListView listView;

    CURRENT cur;

    ArrayList sym = new ArrayList();

    String sort_by[] = {"Sort by","Default","Symbol","Price","Change","Change(%)"};
    String order[] = {"Order","Ascending","Descending"};
    String tag="default";
    String sort_tag="default";

    //test favorite list
    String test_symbol[]={"abc","c","ACB"};
    Double test_price[]={991.24,258.22,379.22};
    String test_change[]={"3.01(4.25%)","2.25(-2.25%)","2.11(1.36%)"};

    private ArrayAdapter adapter;
    private ArrayList<HashMap<String, Object>> favorite_list = new ArrayList<>();

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        setContentView(R.layout.search_stock_jy);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        //main search view
        symbolText = findViewById(R.id.SymbolText);
        search = findViewById(R.id.search);
        clear = findViewById(R.id.clear);

        //with no "[]",use this: object
        symbolText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

                String auto_symbol = symbolText.getText().toString();
                //sym.clear();
                RequestQueue requestQueue = Volley.newRequestQueue(getApplicationContext());
//                //https://jsonplaceholder.typicode.com/users
                adapter = new ArrayAdapter(getApplicationContext(),android.R.layout.simple_list_item_1,sym);
                symbolText.setAdapter(adapter);
                symbolText.setThreshold(1);
                //http://jiayiren-stock.us-west-1.elasticbeanstalk.com/stockinfo.php?symbol=%22a%22
                 url = "http://stock4jyr.us-west-1.elasticbeanstalk.com/phplink/data/auto.php?symbol=%22"+auto_symbol+"%22";
                update();
//                JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, url, null,
//                        new Response.Listener<JSONObject>() {
//                            @Override
//                            public void onResponse(JSONObject response) {
//                                try {
//
//                                    JSONArray name = response.getJSONArray("full_symbol");
//                                    for (int i = 0; i < name.length(); i++) {
//
//                                        String s = name.getString(i);
//                                        sym.add(s);
//                                        Log.d("Name", s);
//                                    }
//
//                                } catch (JSONException e) {
//                                    e.printStackTrace();
//                                }
//                            }
//                        },
//                        new Response.ErrorListener() {
//                            @Override
//                            public void onErrorResponse(VolleyError error) {
//                                Toast.makeText(getApplicationContext(),"Nothing found!", Toast.LENGTH_SHORT);
//
//                            }
//                        });
//                requestQueue.add(jsonObjectRequest);
                //for autocomplete list

//
            }
            @Override
            public void afterTextChanged(Editable editable) {

            }
        });

        search.setOnClickListener(new View.OnClickListener() {
                                      @Override
                                      public void onClick(View view) {
                                          if(symbolText.getText().length()==0){
                                              Toast.makeText(getApplicationContext(),"Please enter a stock name or symbol",Toast.LENGTH_LONG).show();
                                          }
                                          else{
                                              String temp_s = symbolText.getText().toString();
                                              stockSymbol =  temp_s.split("-")[0];
                                              Toast.makeText(getApplicationContext(),symbolText.getText(),Toast.LENGTH_LONG).show();
                                              Intent toStock = new Intent(getApplicationContext(), show_stock.class);

                                              toStock.putExtra("StockSymbol",stockSymbol);
                                              toStock.putExtra("fromfavorite",false);
                                              if(favorite_list!=null) {
                                                  toStock.putExtra("favoriteList",(Serializable) favorite_list);
                                              }
                                              //toStock.putExtra("favoriteList", favorite_list);
                                              startActivity(toStock);
                                              finish();

                                               }
                                      }
                                  });
        clear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(getApplicationContext(), "hi", Toast.LENGTH_LONG).show();
                symbolText.setText(" ");

            }
        });
        //-----------------------------------------------
        Intent intent = getIntent();
        ArrayList<HashMap<String,Object>> data = (ArrayList<HashMap<String,Object>>) intent
                .getSerializableExtra("data");//获取intent中的数据
        favorite_list = data;

        //sort list
        final Spinner sp_sortBy = findViewById(R.id.sort_by);
        final Spinner sp_order = findViewById(R.id.order);
        ArrayAdapter<String> adapter_sortby = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,sort_by);
        ArrayAdapter<String> adapter_order = new ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,order);
        sp_sortBy.setAdapter(adapter_sortby);
        sp_order.setAdapter(adapter_order);

        //add initial list
        listView = findViewById(R.id.listView_favorite);
if(favorite_list==null){
    sort_tag="none";
        }
        else if(favorite_list.size()>0) {
    //arrayList = cur.getFav();
//    for (int i = 0; i < favorite_list.size(); i++) {
//        HashMap<String, Object> map = new HashMap<>();
//        map.put("symbol", test_symbol[i]);
//        map.put("price", test_price[i]);
//        map.put("change", test_change[i]);
//        favorite_list.add(map);
//    }
}
        sp_sortBy.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                //view.setEnabled(false);
                //view.setClickable(false);
//                    view.setEnabled(false);
//                    view.setFocusable(false);
//                    view.setSelected(false);
                if(favorite_list!=null) {
                    switch (i) {
                        //sortby default

                        case 0:break;
                        case 1:
                            sort_tag = "default";
                            break;
                        //symbol alpha
                        case 2:
                            sort_tag = "symbol";
                            //getSort_symbol(tag);
                            break;
                        //price
                        case 3:
                            sort_tag = "price";
                            break;
                        //change
                        case 4:
                            sort_tag = "change";
                            break;
                        //change%
                        case 5:
                            sort_tag = "changep";
                            break;
                    }

                }
                else if(favorite_list==null){
                    view.setEnabled(false);
                }
//                    view.setBackgroundColor(getResources().getColor(R.color.colorWhite,null));
                //adapterView.getSelectedView().setEnabled(false);
                //adapterView.getSelectedView().setClickable(false);
//                    adapterView.getSelectedView().setBackgroundColor(getResources().getColor(R.color.colorPrimaryDark,null));

            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }

        });

        sp_order.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                    if(sort_tag!="none"){
                        switch (i){
                            case 0:;break;
                            case 1: getSortUp(sort_tag);break;
                            case 2: getSortDown(sort_tag);
                                break;
                        }
                        SimpleAdapter fav_adapter = new SimpleAdapter(
                                getApplication(),
                                favorite_list,
                                R.layout.favorite_list,
                                new String[]{"symbol","price","change"},
                                new int[]{R.id.ItemSymbol,R.id.ItemPrice,R.id.ItemChange}
                        );
                        listView.setAdapter(fav_adapter);
                        registerForContextMenu(listView);
                    }
                    else{
                        view.setEnabled(false);
                    }
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });

        //listdelete
//        listView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
//            @Override
//            public boolean onItemLongClick(AdapterView<?> adapterView, View view, int i, long l) {


//                favorite_list.remove(i);
//                SimpleAdapter fav_adapter = new SimpleAdapter(
//                        getApplication(),
//                        favorite_list,
//                        R.layout.favorite_list,
//                        new String[]{"symbol","price","change"},
//                        new int[]{R.id.ItemSymbol,R.id.ItemPrice,R.id.ItemChange}
//                );
//                listView.setAdapter(fav_adapter);
                //view.setVisibility(View.GONE);
//                return false;
//            }
//        });


//        JsonArrayRequest jsonArrayRequest = new JsonArrayRequest(Request.Method.GET, url, null,
//                new Response.Listener<JSONArray>() {
//                    @Override
//                    public void onResponse(JSONArray response) {
//                        tv.setText("JYresponse" + response.toString());
//                        Toast.makeText(getApplicationContext(), response.toString(), Toast.LENGTH_SHORT);
//                        StringBuilder names = new StringBuilder();
//                        names.append("Parses names from res: ");
//                        try {
//                            for (int i = 0; i < response.length(); i++) {
//                                JSONObject jresponse = response.getJSONObject(i);
//                                String name = jresponse.getString("name");
//                                names.append(name).append(",");
//                                Log.d("Name", name);
//                            }
//                            names.deleteCharAt(names.length() - 2);
//                            tryresponse.setText(names.toString());
//                        } catch (JSONException e) {
//                            e.printStackTrace();
//                        }
//                    }
//                },
//                new Response.ErrorListener() {
//                    @Override
//                    public void onErrorResponse(VolleyError error) {
//                        Toast.makeText(getApplicationContext(),"Nothing found!", Toast.LENGTH_SHORT);
//                    }
//                });
//        requestQueue.add(jsonArrayRequest);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                System.out.println("item click");
                Intent toStock = new Intent(getApplicationContext(), show_stock.class);
                stockSymbol=(String)favorite_list.get(i).get("symbol");
                toStock.putExtra("StockSymbol",stockSymbol);
                toStock.putExtra("from_favorite",true);
                toStock.putExtra("favoriteList",(Serializable) favorite_list);
                startActivity(toStock);
                finish();
            }
        });

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_stock_jy, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        int id = item.getItemId();

        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onStart() {
        super.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    private void getSortUp(String tag){
        System.out.println("tag:"+tag);
        switch (tag){
        case "symbol":
            Collections.sort(favorite_list, new Comparator<Object>() {
            @Override
            public int compare(Object o, Object t1) {
                HashMap a = (HashMap)o;
                HashMap b = (HashMap)t1;
                String symbol_a = (String)a.get("symbol");
                String symbol_b = (String)b.get("symbol");
                symbol_a = symbol_a.toUpperCase();
                symbol_b=symbol_b.toUpperCase();
                return symbol_a.compareTo(symbol_b)>0 ? 1 :-1;
                //return symbol_a.compareTo(symbol_b)<0 ? 1 :-1;
            }
        });break;
            case "price":
                Collections.sort(favorite_list, new Comparator<Object>() {
                    @Override
                    public int compare(Object o, Object t1) {
                        HashMap a = (HashMap) o;
                        HashMap b = (HashMap) t1;
//                        Double symbol_a = (Double) a.get("price");
//                        Double symbol_b = (Double) b.get("price");
                        String symbol_a = (String) a.get("price");
                        String symbol_b = (String) b.get("price");
                        return symbol_a.compareTo(symbol_b) > 0 ? 1 : -1;
                        //return symbol_a.compareTo(symbol_b)<0 ? 1 :-1;
                    }
                });break;
            case "change":
                Collections.sort(favorite_list, new Comparator<Object>() {
                @Override
                public int compare(Object o, Object t1) {
                    HashMap a = (HashMap) o;
                    HashMap b = (HashMap) t1;
                    String symbol_a = (String) a.get("change");
                    String symbol_b = (String) b.get("change");
                    return symbol_a.compareTo(symbol_b) > 0 ? 1 : -1;
                    //return symbol_a.compareTo(symbol_b)<0 ? 1 :-1;
                }
            });break;
            case "changep":
                Collections.sort(favorite_list, new Comparator<Object>() {
                @Override
                public int compare(Object o, Object t1) {
                    HashMap a = (HashMap) o;
                    HashMap b = (HashMap) t1;
                    String symbol_a = (String) a.get("change");
                    String symbol_b = (String) b.get("change");
                    String sub_a = symbol_a.substring(symbol_a.indexOf("(")+1,symbol_a.indexOf("%"));
                    String sub_b = symbol_b.substring(symbol_b.indexOf("(")+1,symbol_b.indexOf("%"));
                    return sub_a.compareTo(sub_b) > 0 ? 1 : -1;
                    //return symbol_a.compareTo(symbol_b)<0 ? 1 :-1;
                }
            });break;
            case "default":;break;
        }

    }
    private void getSortDown(String tag){
        System.out.println("tagdown:"+tag);
        switch (tag) {
            case "symbol":
                Collections.sort(favorite_list, new Comparator<Object>() {
                    @Override
                    public int compare(Object o, Object t1) {
                        HashMap a = (HashMap)o;
                        HashMap b = (HashMap)t1;
                        String symbol_a = (String)a.get("symbol");
                        String symbol_b = (String)b.get("symbol");
                        symbol_a = symbol_a.toUpperCase();
                        symbol_b=symbol_b.toUpperCase();
                        //return symbol_a.compareTo(symbol_b)>0 ? 1 :-1;}
                        return symbol_a.compareTo(symbol_b)<0 ? 1 :-1;

                    }
                });
                break;
            case "price":
                Collections.sort(favorite_list, new Comparator<Object>() {
                    @Override
                    public int compare(Object o, Object t1) {
                        HashMap a = (HashMap) o;
                        HashMap b = (HashMap) t1;
//                        Double symbol_a = (Double) a.get("price");
//                        Double symbol_b = (Double) b.get("price");
                        String symbol_a = (String) a.get("price");
                        String symbol_b = (String) b.get("price");
                        //return symbol_a.compareTo(symbol_b) > 0 ? 1 : -1;
                        return symbol_a.compareTo(symbol_b)<0 ? 1 :-1;
                    }
                });break;
            case "change":Collections.sort(favorite_list, new Comparator<Object>() {
                @Override
                public int compare(Object o, Object t1) {
                    HashMap a = (HashMap) o;
                    HashMap b = (HashMap) t1;
                    String symbol_a = (String) a.get("change");
                    String symbol_b = (String) b.get("change");
                    //return symbol_a.compareTo(symbol_b) > 0 ? 1 : -1;
                    return symbol_a.compareTo(symbol_b)<0 ? 1 :-1;
                }
            });break;
            case "changep":
                Collections.sort(favorite_list, new Comparator<Object>() {
                    @Override
                    public int compare(Object o, Object t1) {
                        HashMap a = (HashMap) o;
                        HashMap b = (HashMap) t1;
                        String symbol_a = (String) a.get("change");
                        String symbol_b = (String) b.get("change");
                        String sub_a = symbol_a.substring(symbol_a.indexOf("(")+1,symbol_a.indexOf("%"));
                        String sub_b = symbol_b.substring(symbol_b.indexOf("(")+1,symbol_b.indexOf("%"));
                        return sub_a.compareTo(sub_b) > 0 ? 1 : -1;
                        //return symbol_a.compareTo(symbol_b)<0 ? 1 :-1;
                    }
                });break;
            case "default":
                ;
                break;
        }
    }

    int item_id;
    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
        //获取对应的item的positon
        AdapterView.AdapterContextMenuInfo info = (AdapterView.AdapterContextMenuInfo) menuInfo;
        item_id = info.position;
        //设置菜单布局
        MenuInflater inflater = new MenuInflater(this);
        inflater.inflate(R.menu.delete,menu);
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {

        switch (item.getItemId()){
            case R.id.yes:
                favorite_list.remove(item_id);
                SimpleAdapter fav_adapter = new SimpleAdapter(
                        getApplication(),
                        favorite_list,
                        R.layout.favorite_list,
                        new String[]{"symbol","price","change"},
                        new int[]{R.id.ItemSymbol,R.id.ItemPrice,R.id.ItemChange}
                );
                listView.setAdapter(fav_adapter);

                break;
        }
        return true;
    }


    private void update(){
       UpdateTextTask updateTextTask = new UpdateTextTask(getApplicationContext());
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

            RequestQueue requestQueue = Volley.newRequestQueue(getApplicationContext());
            JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, url, null,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            try {

                                JSONArray name = response.getJSONArray("full_symbol");
                                for (int i = 0; i < 5; i++) {

                                    String s = name.getString(i);
                                    sym.add(s);
                                    Log.d("Name", s);
                                }

                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                            adapter.notifyDataSetChanged();
                        }
                    },
                    new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            Toast.makeText(getApplicationContext(),"Nothing found!", Toast.LENGTH_SHORT);

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
            //progressBar.setVisibility(View.GONE);
            Toast.makeText(context,"finish",Toast.LENGTH_SHORT).show();

        }

        /**
         * 在publishProgress()被调用以后执行，publishProgress()用于更新进度
         */
        @Override
        protected void onProgressUpdate(Integer... values) {
         //   progressBar.setVisibility(View.VISIBLE);
        }
    }

}
