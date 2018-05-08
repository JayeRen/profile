<?php
//this serves for autocomplete
//-----------------------
$json=$_GET["symbol"];
$symbol = json_decode($json);
//----------------------
$url="http://dev.markitondemand.com/MODApis/Api/v2/Lookup/json?input=".$symbol;
$json = file_get_contents($url);
$json_val = json_decode($json);

$full=array();
if(!isset($json_val["0"]->{"Symbol"})):
  $result="1";  
elseif(isset($json_val["0"]->{"Symbol"})):
    foreach($json_val as $val){
        $index=$val->{"Symbol"};
        $name=$val->{"Name"};
        $exchg=$val->{"Exchange"};
        $txt=$index."-".$name."(".$exchg.")";      
        array_push($full,$txt);
    }
endif;
$result["full_symbol"]=$full;
echo json_encode($result);

//$r = json_encode($result);
//echo $r;
?>