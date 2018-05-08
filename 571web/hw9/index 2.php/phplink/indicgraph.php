<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8">
        <script src="http://code.jquery.com/jquery-3.2.1.js" integrity="sha256-DZAnKJ/6XZ9si04Hgrsxu/8s717jcIzLy3oi35EouyE=" crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.3/umd/popper.min.js" integrity="sha384-vFJXuSJphROIrBnz7yo7oB41mKfc8JzQZiCq4NCceLEaO4IHwicKwpJf9c9IpFgh" crossorigin="anonymous"></script>
        
        
        <script src="https://code.highcharts.com/stock/highstock.js"></script>
    <script src="https://code.highcharts.com/stock/modules/exporting.js"></script>
        <script src="http://code.highcharts.com/modules/offline-exporting.js"></script>
    </head>
    <body>
<!--        <button id="click">click</button>-->
        <div id="container">
        </div>
    </body>
</html>
<script type="text/javascript">
    $(document).ready(function(){
//        $('#click').click(function(){
////    window.alert("js success!");
//       // Android.showToast("js connection~");
//        var s= Android.getAddress();
//            Android.showToast(s.toString());
//            createChart(s);
//    });
     
        //$s= Android.getAddress();
         $symbol = Android.getSymbol();
        $ind = Android.getIndicator();
       $s="data/stockgraph.php?symbol=%22"+$symbol+"%22&ind=%22"+$ind+"%22";
        
        //$ind_url = "http://jiayiren-stock.us-west-1.elasticbeanstalk.com/price&history.php?symbol=%22aapl%22";
       
        //$s="phplink/stockgraph.php?symbol=%22aapl%22&ind=%22SMA%22";
        $.ajax({
            url:$s,
            async:true,
            contentType:"application/json; charset=utf-8",
//            data: {
//                symbol: JSON.stringify("aapl")
//            },
            dataType: "json",
            cache: true,
            success: function(data){
              creategraph(data);  
            }
        })
      
    });
    
     function creategraph(ind) {
        var symbol = ind["symbol"].toUpperCase();
        var title = ind["title"];
        var date = ind["date"];
        var ini = ind["indicator"].match(/\((\S*)\)/)[1];
        var sub_name = ind["sub_name"];
        var ind_stat = ind["stat"];
        var statis = ind_stat[sub_name[0]];
        var index = ini + "_con";
        var chart = Highcharts.chart("container", {

            title: {
                text: title
            },
            subtitle: {
                text: '<a href="https://www.alphavantage.co/">' + 'Source:Alpha Vantage</a>'
            },
            xAxis: {
                categories: date,
                tickWidth: 1,
                tickInterval: 3,
                crosshairs: true

            },
            yAxis: {
                title: {
                    text: ini
                }
            },
            legend: {
                layout: 'vertical',
                align: 'center',
                verticalAlign: 'bottom'
            },
            plotOptions: {
                series: {
                    connectNulls: true
                },
                spline: {
                    marker: {
                        enabled: true,
                        radius: 2

                    }
                }
            },
            series: [{
                type: 'spline',
                name: symbol + " " + sub_name[0],
                data: statis
            }]
        });
        if (sub_name.length > 1) {
            for (i = 1; i < sub_name.length; i++) {
                chart.addSeries({
                    type: 'spline',
                    name: symbol + " " + sub_name[i],
                    data: ind_stat[sub_name[i]]
                });
            }
        }



    }
</script>