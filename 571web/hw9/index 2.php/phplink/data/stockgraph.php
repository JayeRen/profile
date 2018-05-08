<?php

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
$json=$_GET["symbol"];
$symbol = json_decode($json);
$ind=$_GET["ind"];
$indicator = json_decode($ind);
//echo json_encode($obj);
//$symbol = $obj->input;
//-----------------------------------------------------------------
switch($indicator){
    case "SMA":
        $price_url="".$symbol."";
        $sma_url="https://www.alphavantage.co/query?function=SMA&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
        $result=dealind("SMA",$sma_url);
        ;break;
        
    case "EMA":
        $ema_url="https://www.alphavantage.co/query?function=EMA&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
        $result=dealind("EMA",$ema_url);
        ;break;
        
    case "STOCH":
        $stoch_url="https://www.alphavantage.co/query?function=STOCH&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&slowkmatype=1&slowdmatype=1&apikey=ZH0JISPRJNEON69U";
        $result=dealind("STOCH",$stoch_url);
        ;break;
        
    case "RSI":
        $rsi_url="https://www.alphavantage.co/query?function=RSI&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
        $result=dealind("RSI",$rsi_url);
        ;break;
        
    case "ADX":
        $adx_url="https://www.alphavantage.co/query?function=ADX&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
        $result=dealind("ADX",$adx_url);
        ;break;
        
    case "CCI":
        $cci_url="https://www.alphavantage.co/query?function=CCI&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
        $result=dealind("CCI",$cci_url);
        ;break;
        
    case "BBANDS":
        $bbands_url="https://www.alphavantage.co/query?function=BBANDS&symbol=".$symbol."&interval=daily&time_period=5&series_type=open&nbdevup=3&nbdevdn=3&apikey=ZH0JISPRJNEON69U";
        $result=dealind("BBANDS",$bbands_url);
        ;break;
        
    case "MACD":
        $macd_url="https://www.alphavantage.co/query?function=MACD&symbol=".$symbol."&interval=daily&time_period=10&series_type=open&apikey=ZH0JISPRJNEON69U";
        $result=dealind("MACD",$macd_url);
        ;break;
}
//output result
    $r = json_encode($result);
    echo $r;
?>