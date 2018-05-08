package cn.jiayibuer.hw9v1;

import android.content.Intent;
import android.support.design.widget.TabLayout;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.FragmentActivity;
import android.support.v4.internal.view.SupportMenu;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;

import android.webkit.ValueCallback;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;

import com.facebook.FacebookSdk;
import com.facebook.appevents.AppEventsLogger;

public class show_stock extends AppCompatActivity implements CURRENT.callbackValue{


    private SectionsPagerAdapter mSectionsPagerAdapter;

    private ViewPager mViewPager;

    private String symbol;
    private boolean from_favorite;
    private  ArrayList<HashMap<String,Object>> favoriteList =new ArrayList<>();

    ArrayList<HashMap<String, Object>> arrayList = new ArrayList<>();
   // HashMap<String, Object> map = new HashMap<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_show_stock);

        FacebookSdk.sdkInitialize(getApplicationContext());
        AppEventsLogger.activateApp(this);

        Intent getF = getIntent();
        symbol = getF.getExtras().getString("StockSymbol");
        from_favorite = getF.getExtras().getBoolean("from_favorite");
        if(getF.getSerializableExtra("favoriteList")!=null){
            favoriteList = (ArrayList<HashMap<String, Object>>) getF
                    .getSerializableExtra("favoriteList");
        }
        Toolbar toolbar = findViewById(R.id.toolbar);
        String symbol_tool = getResources().getString(R.string.stock_symbol);
        symbol_tool = String.format(symbol_tool,symbol);
        toolbar.setTitle(symbol_tool);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);




        // Create the adapter that will return a fragment for each of the three
        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());
        mSectionsPagerAdapter.addFragment(new CURRENT(),"CURRENT");
        mSectionsPagerAdapter.addFragment(new HISTORICAL(),"HISTORICAL");
        mSectionsPagerAdapter.addFragment(new NEWS(),"NEWS");

        // Set up the ViewPager with the sections adapter.
        mViewPager = findViewById(R.id.container);
        mViewPager.setAdapter(mSectionsPagerAdapter);

        TabLayout tabLayout = findViewById(R.id.tabs);

        mViewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));
        tabLayout.addOnTabSelectedListener(new TabLayout.ViewPagerOnTabSelectedListener(mViewPager));


    }
    @Override
    public boolean onOptionsItemSelected(MenuItem menuItem) {
        if (menuItem.getItemId() == android.R.id.home) {
            Intent intent = new Intent(this, StockJY.class);
            intent.putExtra("data", (Serializable) favoriteList);
            startActivity(intent);
            finish();
        }
        return super.onOptionsItemSelected(menuItem);
    }

    public class SectionsPagerAdapter extends FragmentPagerAdapter {

        public SectionsPagerAdapter(FragmentManager fm) {
            super(fm);
        }

        private final ArrayList<Fragment> mFragmentList=new ArrayList<>();
        private final ArrayList<String> mFragmentTitleList=new ArrayList<>();

        public void addFragment(Fragment fragment,String title){
            mFragmentList.add(fragment);
            mFragmentTitleList.add(title);

        }

        @Override
        public Fragment getItem(int position) {
            return mFragmentList.get(position);
        }

        @Override
        public int getCount() {
            // Show 3 total pages.
            return mFragmentList.size();
        }

    }

    public HashMap<String,Object> getTitles(){

        HashMap map=new HashMap<String,Object>();
        map.put("symbol",symbol);
        map.put("from_favorite",from_favorite);
        map.put("stock_info",arrayList);
        if(favoriteList!=null) {
            map.put("favoriteList", favoriteList);
        }

        return  map;
    }
    @Override
    public void sendDataOfRecord(ArrayList<HashMap<String,Object>> data) {
          favoriteList = data;
//        Intent intent = new Intent();
//        intent.putExtra("data", (Serializable) data);
//        intent.putExtra("kinds", kinds);
//        intent.setClass(this,StockJY.class);
//        startActivity(intent);
//        finish();
    }

}
