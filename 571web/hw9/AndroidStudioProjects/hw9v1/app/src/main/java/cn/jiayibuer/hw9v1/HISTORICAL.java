package cn.jiayibuer.hw9v1;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import java.util.HashMap;


public class HISTORICAL extends android.support.v4.app.Fragment {

    private String ht_symbol;

    private View view;
    private WebView webview;
    private String ht_url="http://stock4jyr.us-west-1.elasticbeanstalk.com/phplink/historical.php";
    private WebSettings webSettings;

    public HISTORICAL() {
        // Required empty public constructor
    }



    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

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
        HashMap map=((show_stock) context).getTitles();
        ht_symbol = (String)map.get("symbol");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        view= inflater.inflate(R.layout.fragment_historical, container, false);

        webview = view.findViewById(R.id.webview_historical);
        webview.setWebViewClient(new WebViewClient());

        webSettings= webview.getSettings();
        webSettings.setJavaScriptEnabled(true);

        webview.setWebChromeClient(new WebChromeClient());
        webview.addJavascriptInterface(new WebAppInterface(getActivity(),ht_symbol),"Android");

        webview.loadUrl(ht_url);
        return view;
    }

    // TODO: Rename method, update argument and hook method into UI event



    @Override
    public void onDetach() {
        super.onDetach();
       // mListener = null;
    }

}
