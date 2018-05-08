package cn.jiayibuer.hw9v1;

import android.content.Context;
import android.webkit.JavascriptInterface;
import android.widget.Toast;

/**
 * Created by osx on 24/11/2017.
 */

public class WebAppInterface extends StockJY{
    private String ht_symbol;
    private Context context;
    private String ind_url;
    private String ind;

    public WebAppInterface(Context context, String s){
        this.context = context;
        this.ht_symbol =s;
    }

    public WebAppInterface(Context context, String s,String ind){
        this.context = context;
        this.ht_symbol =s;
        this.ind = ind;
    }
    @JavascriptInterface
    public void showToast(String mes){
        Toast.makeText(context,mes,Toast.LENGTH_LONG).show();
    }
    @JavascriptInterface
    public String getSymbol(){
        //ind_url = "http://jiayiren-stock.us-west-1.elasticbeanstalk.com/price&history.php?symbol=%22"+ht_symbol+"%22";
        //ind_url="phplink/price&history.php?symbol=%22"+ht_symbol+"%22";
        return ht_symbol;
        //return ind_url;
    }
    @JavascriptInterface
    public String getIndicator(){
        return ind;
    }

}
