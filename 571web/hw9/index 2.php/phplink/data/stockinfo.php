<?php 
//serve for basic stock info

$result = array();

$json=$_GET["symbol"]; 
$symbol = json_decode($json);
$url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=".$symbol."&apikey=ZH0JISPRJNEON69U";

$json = file_get_contents($url);
$json_val = json_decode($json);
if(isset($json_val->{"Error Message"})):
  //$result="";  
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
endif;
//output result
    $r = json_encode($result);
    echo $r;
?>