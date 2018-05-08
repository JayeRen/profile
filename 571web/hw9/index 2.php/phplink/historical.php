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
        $symbol= Android.getSymbol();
        //$ind_url = "http://jiayiren-stock.us-west-1.elasticbeanstalk.com/price&history.php?symbol=%22aapl%22";
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
              createChart(data.history);  
            }
        })
      
    });
    
    function createChart(ind) {
        var symbol = ind.symbol;
        var date = ind.date;
        var price =ind.price;
//        var symbol="AAPL";
//        var date = [1499299200,1499385600,1499644800,1499731200,1499817600,1499904000,1499990400,1500249600,1500336000,1500422400,1500508800,1500595200,1500854400,1500940800,1501027200,1501113600,1501200000,1501459200,1501545600,1501632000,1501718400,1501804800,1502064000,1502150400,1502236800,1502323200,1502409600,1502668800,1502755200,1502841600,1502928000,1503014400,1503273600,1503360000,1503446400,1503532800,1503619200,1503878400,1503964800,1504051200,1504137600,1504224000,1504569600,1504656000,1504742400,1504828800,1505088000,1505174400,1505260800,1505347200,1505433600,1505692800,1505779200,1505865600,1505952000,1506038400,1506297600,1506384000,1506470400,1506556800,1506643200,1506902400,1506988800,1507075200,1507161600,1507248000,1507507200,1507593600,1507680000,1507766400,1507852800,1508112000,1508198400,1508284800,1508371200,1508457600,1508716800,1508803200,1508889600,1508976000,1509062400,1509321600,1509408000,1509494400,1509580800,1509667200,1509926400,1510012800,1510099200,1510185600,1510272000,1510531200,1510617600,1510704000,1510790400,1510876800,1511136000,1511222400,1511308800,1511481600];
//        var price = ["142.7300","144.1800","145.0600","145.5300","145.7400","147.7700","149.0400","149.5600","150.0800","151.0200","150.3400","150.2700","152.0900","152.7400","153.4600","150.5600","149.5000","148.7300","150.0500","157.1400","155.5700","156.3900","158.8100","160.0800","161.0600","155.3200","157.4800","159.8500","161.6000","160.9500","157.8600","157.5000","157.2100","159.7800","159.9800","159.2700","159.8600","161.4700","162.9100","163.3500","164.0000","164.0500","162.0800","161.9100","161.2600","158.6300","161.5000","160.8600","159.6500","158.2800","159.8800","158.6700","158.7300","156.0700","153.3900","151.8900","150.5500","153.1400","154.2300","153.2800","154.1200","153.8100","154.4800","153.4800","155.3900","155.3000","155.8400","155.9000","156.5500","156.0000","156.9900","159.8800","160.4700","159.7600","155.9800","156.2500","156.1700","157.1000","156.4100","157.4100","163.0500","166.7200","169.0400","166.8900","168.1100","172.5000","174.2500","174.8100","176.2400","175.8800","174.6700","173.9700","171.3400","169.0800","171.1000","170.1500","169.9800","173.1400","174.9600","174.9700"];
        var data=[];
        for(i=0;i< date.length;i++){
            date[i]=parseInt(date[i]+'000');
            price[i]=parseFloat(parseFloat(price[i]).toFixed(2));
            data[i]=[date[i],price[i]];
        }
        
        
        Highcharts.stockChart('container', {


        rangeSelector: {
            selected: 1
        },

        title: {
            text: symbol + ' Stock Price'
        },
        xAxis:{
            categories: date
        },
        series: [{
            name: symbol,
            data: data,
            tooltip: {
                valueDecimals: 2
            }
        }]
    });
}
</script>