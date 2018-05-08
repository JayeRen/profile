<?php
//-----------------------------------------------------------------
//set return result,receive symbol;
$result = array();

//$json1 = file_get_contents("php://input");
$json=$_GET["symbol"];
$symbol = json_decode($json);
//echo json_encode($obj);
//$symbol = $obj->input;
//-----------------------------------------------------------------
$url = "https://seekingalpha.com/api/sa/combined/AAPL.xml";
//$url = "news.xml";
$xml_news=simplexml_load_file($url);
$news_channel = $xml_news -> channel;
ini_set('user_agent','Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; 4399Box.560; .NET4.0C; .NET4.0E)');
$count=0;           
foreach($news_channel->item as $item){
    if($count<5):  
            $news=array();
          $link=$item->link;
          $pubDate = strstr($item->pubDate,"-",true);
            //how to get tag w/ namespaces!annoying
          $ns=$item->getNameSpaces(true);
          
          $news_author = $item->children($ns['sa'])->author_name;
//          $news_title = "news".($count+1);
//        $result[$news_title]="<div class='card bg-light mb-3' style='width:100%'><div class='card-body'><h4 class='card-title'><a href='".$link."' target='_blank'>".$item->title."</a></h4><p class='card-text'>".$news_author."</p><p class='card-text'>".$pubDate."</p></div></div>";
            array_push($news,$item->title);
            array_push($news,$news_author);
            array_push($news,$pubDate);
            array_push($news,$link);
    $result["news".($count+1)]=$news;
    else: break;
    endif;
    $count++;
}
//output result
    $r = json_encode($result);
    echo $r;
?>