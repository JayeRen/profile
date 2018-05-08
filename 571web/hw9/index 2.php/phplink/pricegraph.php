
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
<script>
    $(document).ready(function(){
//        $('#click').click(function(){
////    window.alert("js success!");
//       // Android.showToast("js connection~");
//        var s= Android.getAddress();
//            Android.showToast(s.toString());
//            createChart(s);
//    });
        $symbol = Android.getSymbol();
        
        //$s= Android.getAddress();
        //$ind_url = "http://jiayiren-stock.us-west-1.elasticbeanstalk.com/price&history.php?symbol=%22aapl%22";
        //$s = "phplink/price&history.php?symbol=%22aapl%22"
        $s = "data/price&history.php?symbol=%22"+$symbol+"%22";
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
              createprice(data.pricestock);  
            }
        })
      
    });
    
function createprice(ind){
            var symbol = ind["symbol"].toUpperCase();
            var dates = ind["date"];
            var volume = ind["volume"];
            var price = ind["price"];
            var sp = new Array();
            var r_sp = new Array();

            var len = dates.length;
            var r_min, r_max;
            //reverse vol follow everyday
            for (var i = 0; i < len; i++) {
                volume[i] = parseInt(volume[i]);
                
                price[i] = parseFloat(parseFloat(price[i]).toFixed(2));

                if (i == 0) {
                    r_min = price[0];
                    r_max = volume[0];
                } else {
                    r_min = Math.min(r_min, price[i]);
                    r_max = Math.max(r_max, volume[i]);
                }
            }
            var chart1 = Highcharts.chart("container", {
                title: {
                    text: symbol+' Stock Price and Volume'
                },
                subtitle: {
                    text: '<a href="https://www.alphavantage.co/">' + 'Source:Alpha Vantage</a>'
                },
                xAxis: {
                    categories: dates,
                    tickInterval: 7,
                    crosshair: true,
                },
                yAxis: [
                    //first,right
                    {
                        title: {
                            text: 'Volume'
                        },
                        opposite: true,
                        max: r_max * 2

                    }, {
                        title: {
                            text: 'Stock Price'
                        },
                        tickInterval: 5,
                        min: r_min - 5.0
                    }
                ],
                legend: {
                    layout: 'vertical',
                    align: 'center',
                    verticalAlign: 'bottom'
                },
                plotOptions: {
                    series: {
                        connectNulls: true
                    },
                    areaspline: {
                        marker: {
                            enabled: false
                        }
                    }
                },
                series: [{
                    type: 'areaspline',
                    name: 'Price',
                    yAxis: 1,
                    data: price,
                    fillColor: 'RGB(255,136,136)',
                    lineWidth: 1,
                    color: '#FF0000'
                }, {
                    type: 'column',
                    name: 'Volume',
                    data: volume,
                    color: 'white'
                }]
            });
            return chart1;

    }
</script>