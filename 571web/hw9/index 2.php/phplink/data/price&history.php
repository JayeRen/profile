<?php 

$result = array();
//$json1 = file_get_contents("php://input");
$json=$_GET["symbol"];
$symbol = json_decode($json);

//save url as local json:chart.json(delete later)
$url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=".$symbol."&apikey=ZH0JISPRJNEON69U";
//$url="chart.json";
$json = file_get_contents($url);
$json_val = json_decode($json);

if(isset($json_val->{"Error Message"})):
  $result="";  
elseif(isset($json_val->{"Meta Data"})):

//price stock and history stock:
$ps["symbol"]=$symbol;
$ps["date"]=array();
$ps["price"]=array();
$ps["volume"]=array();

$history["symbol"]=$symbol;
$history["date"]=array();
$history["price"]=array();

$date_ = $json_val->{"Meta Data"}->{"3. Last Refreshed"};
$date_ = date("Y-m-d",strtotime($date_));
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

//output result
    $r = json_encode($result);
    echo $r;




?>