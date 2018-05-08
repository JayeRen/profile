package cn.jiayibuer.hw9v1;

import android.util.Log;
import android.widget.AutoCompleteTextView;
import android.widget.SimpleAdapter;
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

/**
 * Created by osx on 22/11/2017.
 */

public class StockDB {
    StockJY stock;

    public String getSymbol(AutoCompleteTextView autoInput){
        String temp_symbol = autoInput.getText().toString();
        System.out.println("original:"+temp_symbol);
        String[] symbol = temp_symbol.split("-");
        System.out.println("aftersplit:"+symbol[0]);
        return symbol[0];

    }
   public ArrayList<HashMap<String, Object>> getStockInfo(String symbol){
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
       final String[] title = {"Stock Symbol","Last Price","Change","TimeStamp","Open","close","Day's Range","Volume"};
       final String[] index = {"sts"         ,"lastprice" ,"change","timestamp","open","close","daysrange"  ,"volume"};
       final ArrayList<HashMap<String,Object>> arrayList = new ArrayList<>();


       //http request
        RequestQueue requestQueue = Volley.newRequestQueue(stock.getApplicationContext());
        //https://jsonplaceholder.typicode.com/users
        //http://jiayiren-stock.us-west-1.elasticbeanstalk.com/stockinfo.php?symbol=%22a%22
//        String url = "http://jiayiren-stock.us-west-1.elasticbeanstalk.com/stockinfo.php?symbol=%22"+symbol+"%22";
        String url = "http://jiayiren-stock.us-west-1.elasticbeanstalk.com/stockinfo.php?symbol=%22"+symbol+"%22";
        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            for(int i=0;i<index.length;i++){
                                HashMap<String,Object> map = new HashMap<>();
                                String index_val = response.getString(index[i]);
                                map.put("Title",title[i]);
                                map.put("Value",index_val);
                                arrayList.add(map);
                            }

                            //Toast.makeText(stock.getApplicationContext(),sts, Toast.LENGTH_SHORT);

                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Toast.makeText(stock.getApplicationContext(),"Nothing found!", Toast.LENGTH_SHORT);

                    }
                });
        requestQueue.add(jsonObjectRequest);
        return  arrayList;
    }


}
