<?php
//-----------------------------------------------------------------
//--------------extra function-----------------------------
//day2:today
function countdays($day1,$day2){
    $day1 = strtotime($day1);
    $day2 = strtotime($day2);
    $ts_diff_days=($day2-$day1)/86400;
    return $ts_diff_days;
}
///////////////////////////////////////
//graph part
//sample json of ind
//deal with indicator data, serve highchart graphs
function dealind($ind_symbol,$url){
    //for return
    $ind= array();
    
    //parse json data
    $s_url=file_get_contents($url);
    $s= json_decode($s_url);
    
    //symbol,indicator,current date
    $ind["symbol"]=$s->{"Meta Data"}->{"1: Symbol"};
    $ind["indicator"]=$s->{"Meta Data"}->{"2: Indicator"};
    $ind["today"]=$s->{"Meta Data"}->{"3: Last Refreshed"};
    
    //second part of data
    $temp_sym = "Technical Analysis: ".$ind_symbol;
    $ind_data = $s->{$temp_sym};
    
    //prepare for new data
    $ind_date = array();
    $ind_subname=array();
    $ind_stat = array();
    //****array_unshift($a,data) can insert data from head instead of tail
    foreach($ind_data as $i_date=>$val){
                    //get half year data
                    if($i_date == $ind["today"]):
                        foreach($val as $sub_data=>$num) {
                            array_unshift($ind_subname,$sub_data);
                            $ind_stat[$sub_data] = array();
                        } 
                    endif;
                    if (countdays($i_date, $ind["today"]) < 366 / 2):
                        //date array
                        foreach($val as $sub_data=>$num) {
                            $temp_data = round($num,2);
                            array_unshift($ind_stat[$sub_data],$temp_data);
                        }
                        $temp_date = explode(' ',$i_date)[0];
                        array_unshift($ind_date,$temp_date);
                    else:
                        break;
                    endif;
    }
    //for several tags: $statis = $ind_stat[$ind_subname[0]];
    //packaging data
    $ind["date"]=$ind_date;
    $ind["sub_name"]=$ind_subname;
    $ind["stat"]=$ind_stat;
    return $ind;
}
//-----------------------------------------------------------------
//set return result,receive symbol;
$result = array();
//$json1 = file_get_contents("php://input");
$json=$_GET["input"];
$symbol = json_decode($json);
//echo json_encode($obj);
//$symbol = $obj->input;
//-----------------------------------------------------------------
//stock table 1

//save url as local json:chart.json(delete later)
$url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=".$symbol."&apikey=ZH0JISPRJNEON69U";
//$url="chart.json";
$json = file_get_contents($url);
$json_val = json_decode($json);
if(isset($json_val->{"Error Message"})):
  $result["error"]="Error: NO record has been found, please enter a valid symbol";  
elseif(isset($json_val->{"Meta Data"})):
    $date_ = $json_val->{"Meta Data"}->{"3. Last Refreshed"};
    $date_ = date("Y-m-d",strtotime($date_));
    $sts= $json_val -> {"Meta Data"} -> {"2. Symbol"};
    $current_date= $json_val -> {"Time Series (Daily)"} -> {$date_};
    $close = $current_date -> {"4. close"};
    $open = $current_date -> {"1. open"};
    $prev_date = date("Y-m-d",strtotime("-1 day",strtotime($date_)));
    if(!isset($json_val ->{"Time Series (Daily)"}->{$prev_date})):
         $prev_date = date("Y-m-d",strtotime("-3 day",strtotime($date_)));
    endif;
    $prev_date = $json_val ->{"Time Series (Daily)"}->{$prev_date};
    $prev_close = $prev_date->{"4. close"};
    $change=round($close-$prev_close,2);
    $change_percent=round(($close-$prev_close)*100/$prev_close,2);
    if($change<0 || $change_percent < 0):
        $arrow_img="http://cs-server.usc.edu:45678/hw/hw8/images/Down.png";
    elseif($change>0 || $change_percent > 0):
                            $arrow_img="http://cs-server.usc.edu:45678/hw/hw8/images/Up.png";
    else: $arrow_img="#";            
    endif;
    $days_range=$current_date->{"3. low"}."-".$current_date->{"2. high"};  
    $volume=number_format($current_date->{"5. volume"});
    $timestamp=$date_;
    
    $result["sts"]=$sts;
    $result["lastprice"]=$close;
    $result["change"]=$change."(".$change_percent."%)";
    $result["timestamp"]=$timestamp;
    $result["open"]=$open;
    $result["close"]=$close;
    $result["daysrange"]=$days_range;
    $result["volume"]=$volume;
    $result["image"]=$arrow_img;

//price stock:
$ps["symbol"]=$symbol;
$ps["date"]=array();
$ps["price"]=array();
$ps["volume"]=array();
$history["symbol"]=$symbol;
$history["date"]=array();
$history["price"]=array();
foreach($json_val -> {"Time Series (Daily)"} as $ts_date=>$ts_value){
    $ts_diff_days=($date_-$ts_date)/86400;
    if($ts_diff_days < 366/2):
        $ts_temp_date=date("Y-m-d",strtotime($ts_date));
        array_unshift($ps["date"],$ts_temp_date);
        if(isset($json_val -> {"Time Series (Daily)"} -> $ts_date)):
            $volume= $json_val -> {"Time Series (Daily)"} -> $ts_date ->{"5. volume"};
            $price= $json_val -> {"Time Series (Daily)"} -> $ts_date ->{"4. close"};
            array_unshift($ps["volume"],$volume);
            array_unshift($ps["price"],$price); array_unshift($history["date"],strtotime($ts_date));
            array_unshift($history["price"],$price);
        else: continue;
        endif;
    else:break;
    endif;
$result["pricestock"]=$ps; 
$result["history"]=$history; 
}
endif;
//-----------------------------------------------------------
//graph part:save data as indicator.json (delete later)
//$g_url="indicator.json";
//$result["sma"]=dealind("SMA",$g_url);

$price_url="".$symbol."";
$sma_url="https://www.alphavantage.co/query?function=SMA&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
$result["sma"]=dealind("SMA",$sma_url);

$ema_url="https://www.alphavantage.co/query?function=EMA&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
$result["ema"]=dealind("EMA",$ema_url);

$stoch_url="https://www.alphavantage.co/query?function=STOCH&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&slowkmatype=1&slowdmatype=1&apikey=ZH0JISPRJNEON69U";
$result["stoch"]=dealind("STOCH",$stoch_url);

$rsi_url="https://www.alphavantage.co/query?function=RSI&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
$result["rsi"]=dealind("RSI",$rsi_url);

$adx_url="https://www.alphavantage.co/query?function=ADX&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
$result["adx"]=dealind("ADX",$adx_url);

$cci_url="https://www.alphavantage.co/query?function=CCI&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
$result["cci"]=dealind("CCI",$cci_url);

$bbands_url="https://www.alphavantage.co/query?function=BBANDS&symbol=".$symbol."&interval=daily&time_period=5&series_type=open&nbdevup=3&nbdevdn=3&apikey=ZH0JISPRJNEON69U";
$result["bbands"]=dealind("BBANDS",$bbands_url);

$macd_url="https://www.alphavantage.co/query?function=MACD&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
$result["macd"]=dealind("MACD",$macd_url);
//
//$sma_url="https://www.alphavantage.co/query?function=SMA&symbol=AAPL&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
//$result["sma"]=dealind("SMA",$sma_url);
//
//$ema_url="https://www.alphavantage.co/query?function=EMA&symbol=AAPL&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
//$result["ema"]=dealind("EMA",$ema_url);
//
//$stoch_url="https://www.alphavantage.co/query?function=STOCH&symbol=AAPL&interval=daily&time_period=10&series_type=open&slowkmatype=1&slowdmatype=1&apikey=ZH0JISPRJNEON69U";
//$result["stoch"]=dealind("STOCH",$stoch_url);
//
//$rsi_url="https://www.alphavantage.co/query?function=RSI&symbol=AAPL&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
//$result["rsi"]=dealind("RSI",$rsi_url);
//
//$adx_url="https://www.alphavantage.co/query?function=ADX&symbol=AAPL&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
//$result["adx"]=dealind("ADX",$adx_url);
//
//$cci_url="https://www.alphavantage.co/query?function=CCI&symbol=AAPL&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
//$result["cci"]=dealind("CCI",$cci_url);
//
//$bbands_url="https://www.alphavantage.co/query?function=BBANDS&symbol=AAPL&interval=daily&time_period=5&series_type=open&nbdevup=3&nbdevdn=3&apikey=ZH0JISPRJNEON69U";
//$result["bbands"]=dealind("BBANDS",$bbands_url);
//
//$macd_url="https://www.alphavantage.co/query?function=MACD&symbol=AAPL&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
//$result["macd"]=dealind("MACD",$macd_url);
//------------------------------------------------------------
//news part: save news data as news.xml(delete later)
$url = "https://seekingalpha.com/api/sa/combined/AAPL.xml";
//$url = "news.xml";
$xml_news=simplexml_load_file($url);
$news_channel = $xml_news -> channel;
ini_set('user_agent','Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; 4399Box.560; .NET4.0C; .NET4.0E)');
$count=0;           
foreach($news_channel->item as $item){
    if($count<5):                   
          $link=$item->link;
          $pubDate = strstr($item->pubDate,"-",true);
            $author="sa:author_name";
          $news_author = $item->pubDate->nextSibling;
          $news_title = "news".($count+1);
        $result[$news_title]="<div class='card bg-light mb-3' style='width:100%'><div class='card-body'><h4 class='card-title'><a href='".$link."' target='_blank'>".$item->title."</a></h4><p class='card-text'>".$news_author."</p><p class='card-text'>".$pubDate."</p></div></div>";
    else: break;
    endif;
    $count++;
}
//echo json_encode($news_author);
//----------------------------------------------------------------
//output result
    $r = json_encode($result);
    echo $r;
?>
