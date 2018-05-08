<!DOCTYPE html>
<html>

<head>

    <!-- Required meta tags -->
    <style>
        body {
            background-image: url("http://cs-server.usc.edu:45678/hw/hw8/images/background.png");

        }

        .full {
            width: 80%;
            margin: 0 auto;
            text-align: center;
        }

        div.big {

            margin-top: 30;
            margin-bottom: 50;
            margin-left: 50;
            margin-right: 50;
            border-radius: 15;
            background-color: white;
        }

    </style>

    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=yes">
    <script src="http://code.jquery.com/jquery-3.2.1.js" integrity="sha256-DZAnKJ/6XZ9si04Hgrsxu/8s717jcIzLy3oi35EouyE=" crossorigin="anonymous"></script>
    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css" integrity="sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb" crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdn.bootcss.com/font-awesome/4.7.0/css/font-awesome.min.css">
    <!-- <link rel="stylesheet" href="font-awesome/css/font-awesome.min.css"> -->

    <!--    <script src="http://cs-server.usc.edu:35019/jquery.js"></script>-->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.3/umd/popper.min.js" integrity="sha384-vFJXuSJphROIrBnz7yo7oB41mKfc8JzQZiCq4NCceLEaO4IHwicKwpJf9c9IpFgh" crossorigin="anonymous"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js" integrity="sha384-alpBpkh1PFOepccYVYDB4do5UnbKysX5WZXm3XxPqe5iKTfUKjNkCk9SaVuEZflJ" crossorigin="anonymous"></script>

    <!--angular api-->
    <!-- Angular Material style sheet -->
    <link rel="stylesheet" href="https://ajax.googleapis.com/ajax/libs/angular_material/1.1.0/angular-material.min.css">

    <!-- Angular Material requires Angular.js Libraries -->
    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.5.5/angular.min.js"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.5.5/angular-animate.min.js"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.5.5/angular-aria.min.js"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.5.5/angular-messages.min.js"></script>

    <!-- Angular Material Library -->
    <script src="https://ajax.googleapis.com/ajax/libs/angular_material/1.1.0/angular-material.min.js"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.6.4/angular.min.js"></script>
    <!-- highchart api   -->
    <!--
    <script src="https://code.highcharts.com/highcharts.js"></script>
    <script src="https://code.highcharts.com/modules/exporting.js"></script>
-->
    <script src="https://code.highcharts.com/stock/highstock.js"></script>
    <script src="https://code.highcharts.com/stock/modules/exporting.js"></script>
    <!--    toggle-->
    <link href="https://gitcdn.github.io/bootstrap-toggle/2.2.2/css/bootstrap-toggle.min.css" rel="stylesheet">
    <script src="https://gitcdn.github.io/bootstrap-toggle/2.2.2/js/bootstrap-toggle.min.js"></script>

</head>

<body id="body" ng-app="myApp" ng-cloak>
    <div class='full'>
        <div class="big rounded">
            <h3>Stock Market Search</h3>
            <!--            novalidate-->
            <form id="needs-validation" ng-submit="$event.preventDefault()">
                <div class="row">

                    <label for="SymbolInput" class="col  col-form-label">Enter Stock Ticker Symbol:</label>

                    <div class="col" ng-controller="SearchCtrl as ctrl" layout="column" ng-cloak>
                        <md-content class="md-padding">
                            <md-autocomplete ng-disabled="ctrl.isDisabled" md-selected-item="ctrl.selectedItem" md-search-text="ctrl.searchText" md-items="item in ctrl.querySearch(ctrl.searchText)" md-item-text="item.display" md-min-length="0" placeholder="e.g.AAPL" required>
                                <!--                                <input type="text" class="form-control" id="sv" required>-->
                                <md-item-template>
                                    <span md-highlight-text="ctrl.searchText" md-highlight-flags="^i" id="SymbolInput">{{item.display}}</span>
                                </md-item-template>
                            </md-autocomplete>
                        </md-content>
                    </div>
                    <div class="col">
                        <button type="submit" class="btn" id="search"><i class="fa fa-search" aria-hidden="true">Get Quote</i></button>
                        <button type="reset" class="btn btn-secondary" id="clear"><i class="fa fa-refresh" aria-hidden="true"></i>
                        Clear</button>
                    </div>
                </div>
            </form>
        </div>
        <hr color=white width=100%>
        <!-- --------favorite form-------- -->
        <div class="big rounded">
            <div class="card" id="favorite-chart" style="display:block">
                <div class="card-header">
                    <div class="font-weight-bold text-left">Favorite List</div>
                    <div class="text-right">
                        Automatic Refresh:<input id="toggle-event" data-toggle="toggle" type="checkbox">
                        <button type="button" class="btn btn-default btn-md" id="refresh"><i class="fa fa-refresh" aria-hidden="true"></i></button>
                        <button type="button" class="btn btn-default btn-md" id="tostock"><i class="fa fa-chevron-right" aria-hidden="true" ></i></button>
                    </div>
                </div>
                <div class="card body">


                    <table id="favorite-table" class="table table-striped table-responsive">
                        <tr>
                            <th>Symbol</th>
                            <th>Stock Price</th>
                            <th>Change(Change Percent)</th>
                            <th>Volume</th>
                            <th></th>
                        </tr>

                    </table>
                </div>
            </div>
            <!--   ---------------------------------------------------------------  
                            main form
------------------------------------------------------------- -->
            <div class="card" id="chart" style="display:none">
                <div class="card-header">
                    <span class="text-left">
                <button type="button" class="btn btn-default btn-md" id="tofav">
            <i class="fa fa-chevron-left" aria-hidden="true"></i>
            </button>
                </span>
                    <span class="font-weight-bold">Stock Detail</span>
                </div>
                <div class="card-body">

                    <ul class="nav nav-pills">
                        <li class="nav-item">
                            <a class="nav-link active" id="cs" data-toggle="pill" href="#stock_info" role="tab" aria-selected="true"><i class="fa fa-tachometer" aria-hidden="true"></i>
Current Stock</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" id="hc" data-toggle="pill" href="#history" role="tab" aria-selected="false"><i class="fa fa-bar-chart" aria-hidden="true"></i>
Historical Charts</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" id="nf" data-toggle="pill" href="#news" role="tab" aria-selected="false"><i class="fa fa-link" aria-hidden="true"></i>
News Feeds</a>
                        </li>
                    </ul>
                    <hr>
                    <div class="tab-content" id="stock_details">
                        <!--   -----------------------------------------------------  
                            tab1:     #stock_info
                    ---------------------------------------------------- -->
                        <div class="tab-pane fade show active" id="stock_info" role="tabpanel" aria-labelledby="cs">

                            <table id='current_stock' class="table table-striped table-responsive">
                                <tr>
                                    <!--   --------------------------------------------  
                            part1:     #info_col     *pic *data
                           ------------------------------------------- -->
                                    <td id="info_col" style="width:50%">
                                        <table id="stock_info" class="table table-responsive">
                                            <tr>
                                                <th scope="row">Stock Detail</th>
                                                <td>
                                                    <button type="button" class="btn btn-default btn-md" id="favorite">
                                        <i class="fa fa-star-o" id="fav-off" aria-hidden="true" style="display:block"></i>
                                        <i class="fa fa-star" 
                                        id="fav-on"
                                        aria-hidden="true" style="display:none"></i>
                                        </button>
                                                    <button type="button" class="btn btn-default btn-md" id="fb"><img src='http://cs-server.usc.edu:45678/hw/hw8/images/facebook.png' height=25 width=25> </button>
                                                </td>
                                            </tr>
                                            <tr>
                                                <th>Stock Ticker Symbol</th>
                                                <td id='sts_val'></td>
                                            </tr>
                                            <tr>
                                                <th>Last Price</th>
                                                <td id='last_price_val'></td>
                                            </tr>
                                            <tr>
                                                <th>Change(Change Percent)</th>
                                                <td id='change_val'></td>
                                            </tr>
                                            <tr>
                                                <th>Timestamp</th>
                                                <td id='timestamp_val'></td>
                                            </tr>
                                            <tr>
                                                <th>Open</th>
                                                <td id=open_val></td>
                                            </tr>
                                            <tr>
                                                <th>Close</th>
                                                <td id='close_val'></td>
                                            </tr>
                                            <tr>
                                                <th>Day's Range</th>
                                                <td id="days_range_val"></td>
                                            </tr>
                                            <tr>
                                                <th>Volume</th>
                                                <td id="volume_val"></td>
                                            </tr>

                                        </table>
                                    </td>
                                    <!--   --------------------------------------------  
                             part2:     #graph_col   *zoom
                         ---------------------------------------------->
                                    <td id="graph_col" style="vertical-align:top">
                                        <div class="container">
                                            <ul class="nav nav-tabs" id="myTab" role="tablist">
                                                <li class="nav-item">
                                                    <a class="nav-link active" id="Price" data-toggle="tab" href="#price_con" role="tab" aria-controls="home" aria-selected="true">Price</a>
                                                </li>
                                                <li class="nav-item">
                                                    <a class="nav-link" id="SMA" data-toggle="tab" href="#SMA_con" role="tab" aria-controls="SMA_con" aria-selected="false">SMA</a>
                                                </li>
                                                <li class="nav-item">
                                                    <a class="nav-link" id="EMA" data-toggle="tab" href="#EMA_con" role="tab" aria-controls="EMA_con" aria-selected="false">EMA</a>
                                                </li>
                                                <li class="nav-item">
                                                    <a class="nav-link" id="STOCH" data-toggle="tab" href="#STOCH_con" role="tab" aria-controls="STOCH_con" aria-selected="false">STOCH</a>
                                                </li>
                                                <li class="nav-item">
                                                    <a class="nav-link" id="RSI" data-toggle="tab" href="#RSI_con" role="tab" aria-controls="RSI_con" aria-selected="false">RSI</a>
                                                </li>
                                                <li class="nav-item">
                                                    <a class="nav-link" id="ADX" data-toggle="tab" href="#ADX_con" role="tab" aria-controls="ADX_con" aria-selected="false">ADX</a>
                                                </li>
                                                <li class="nav-item">
                                                    <a class="nav-link" id="CCI" data-toggle="tab" href="#CCI_con" role="tab" aria-controls="CCI_con" aria-selected="false">CCI</a>
                                                </li>
                                                <li class="nav-item">
                                                    <a class="nav-link" id="BBANDS" data-toggle="tab" href="#BBANDS_con" role="tab" aria-controls="BBANDS_con" aria-selected="false">BBANDS</a>
                                                </li>
                                                <li class="nav-item">
                                                    <a class="nav-link" id="MACD" data-toggle="tab" href="#MACD_con" role="tab" aria-controls="MACD_con" aria-selected="false">MACD</a>
                                                </li>
                                            </ul>
                                            <div class="tab-content" id="myTabContent">
                                                <div class="tab-pane fade show active" id="price_con" role="tabpanel" aria-labelledby="Price"style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>

                                                <div class="tab-pane fade" id="SMA_con" role="tabpanel" aria-labelledby="SMA" style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>

                                                <div class="tab-pane fade" id="EMA_con" role="tabpanel" aria-labelledby="EMA" style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>

                                                <div class="tab-pane fade" id="STOCH_con" role="tabpanel" aria-labelledby="STOCH" style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>

                                                <div class="tab-pane fade" id="RSI_con" role="tabpanel" aria-labelledby="RSI" style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>

                                                <div class="tab-pane fade" id="ADX_con" role="tabpanel" aria-labelledby="ADX" style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>

                                                <div class="tab-pane fade" id="CCI_con" role="tabpanel" aria-labelledby="CCI" style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>

                                                <div class="tab-pane fade" id="BBANDS_con" role="tabpanel" aria-labelledby="BBANDS" style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>

                                                <div class="tab-pane fade" id="MACD_con" role="tabpanel" aria-labelledby="MACD" style='min-width:310px;height: 400px;max-width:100%; margin: 0 auto'></div>
                                            </div>
                                        </div>
                                    </td>
                                </tr>
                            </table>


                        </div>
                        <!--   -----------------------------------------------------  
                            tab2:     #history_info
                    -------------------------------------------------------->
                        <div class="tab-pane fade show active" id="history" role="tabpanel" aria-labelledby="hc">

                        </div>
                        <!--   -----------------------------------------------------  
                            tab3:     #news *format
                    -------------------------------------------------------->
                        <div class="tab-pane fade" id="news" role="tabpanel" aria-labelledby="nf">
                        </div>

                    </div>
                </div>
            </div>
        </div>

    </div>
    
</body>
</html>
<script>
    angular
        .module('myApp', ['ngMaterial'])
        .controller('SearchCtrl', SearchCtrl);

    function SearchCtrl($timeout, $q, $log, $http) {
        var self = this;

        self.simulateQuery = false;
        self.isDisabled = false;
        self.querySearch = querySearch;
        self.newState = newState;

        function newState(state) {
            alert("Sorry! You'll need to create a Constitution for " + state + " first!");
        }

        function querySearch(query) {
            $url = "http://dev.markitondemand.com/MODApis/Api/v2/Lookup/json?input=" + query;
            $http.get($url)
                .then(function(response) {
                    $tag = [];
                    $data = response.data;
                    for ($i = 0; $i < $data.length; $i++) {
                        $txt = $data[$i].Symbol + " - " + $data[$i].Name + ' (' + $data[$i].Exchange + ')';
                        $tag.push($txt);
                    }

                    self.states = $tag.map(function(state) {
                        //alert(state);
                        return {
                            value: state.toLowerCase(),
                            display: state
                        };
                    });
                });
            var results = self.states,
                deferred;
            // var results=$a,deferred;
            if (self.simulateQuery) {
                deferred = $q.defer();
                $timeout(function() {
                    deferred.resolve(results);
                }, Math.random() * 1000, false);
                return deferred.promise;
            } else {
                return results;
            }
        }


    }

    // Your code goes here.
    $(document).ready(function() {

        $("#search").click(function(event) {
            //            var form = document.getElementById('needs-validation');
            ////            if (form.checkValidity() === false) {
            ////                event.preventDefault();
            ////                event.stopPropagation();
            ////            }
            //            form.classList.add('was-validated');
            $symbol = $('#input-0').val();
            $.ajax({
                //type: "GET","http://cs-server.usc.edu:35019/search.php
                //http://cs-server.usc.edu:35019/"
                url: "search.php",
                async: true,
                contentType: "application/json; charset=utf-8",
                data: {
                    input: JSON.stringify($symbol)
                },
                dataType: "json",
                cache: true,
                success: function(data) {
                    //stock_info part
                    //alert(data);
                    //var data = JSON.stringify(data);
                    //$("#input-0").text(data.sts);
                    $('#sts_val').text(data.sts);
                    $('#last_price_val').text(data.lastprice);
                    $('#change_val').html(data.change + "<img src=' " + data.image + "' height=10 width=10>");
                    $('#timestamp_val').text(data.timestamp);
                    $('#open_val').text(data.open);
                    $('#close_val').text(data.close);
                    $('#days_range_val').text(data.daysrange);
                    $('#volume_val').text(data.volume);
                    $('#chart').css("display", "block");
                    $('#favorite-chart').css("display", "none");
                    //localstorage
                    $temp = data.change + "<img src=' " + data.image + "' height=10 width=10>";
                    $fav_txt = "<td>" + data.sts + "</td><td>" + data.lastprice + "</td><td>" + $temp + "</td><td>" + data.volume + "</td><td><button type='button' class='btn btn-default btn-lg'  id='trash" + data.sts + "'><i class='fa fa-trash' aria-hidden='true'></i></button>" + "</td>";

                    //graph
                    $stat = data.ind;
                    createprice(data.pricestock);
                    creategraph(data.sma);
                    //(recover later)
                    creategraph(data.ema);
                    creategraph(data.stoch);
                    creategraph(data.rsi);
                    creategraph(data.adx);
                    creategraph(data.cci);
                    creategraph(data.bbands);
                    creategraph(data.macd);
                    //highstock
                    createChart(data.history);
       
                    //news
                    $('#news').html(data.news1 + data.news2 + data.news3 + data.news4 + data.news5);
                    //favorite function:only exists when symbol exists
                    $fav = 0;
                    $('#favorite').click(function() {
                        if ($fav == 0) {
                            $('#fav-off').css("display", "none");
                            $('#fav-on').css("display", "block");
                            localStorage.setItem("fav_" + data.sts, $fav_txt);

                            $("#favorite-table").append("<tr id='" + data.sts + "_fav'>" + localStorage.getItem("fav_" + data.sts) + "</tr>");
                            $fav = 1;
                        } else if ($fav == 1) {
                            $('#fav-on').css("display", "none");
                            $('#fav-off').css("display", "block");
                            localStorage.removeItem("fav_" + data.sts);
                            $r_tag = '#' + data.sts + "_fav";
                            $($r_tag).remove();
                            $fav = 0;
                        }
                    });
                    $('#fb').click(function() {
                        FB.ui({
                            app_id: '696698673858174',
                            method: 'feed',
                            link: 'https://developers.facebook.com/docs/'
                        }, function(response) {
                            if (response && !response.error_message) {
                                console.log("success");
                                //succeed 
                            } else {
                                //fail 
                                console.log("error");
                            }
                        });
                    });
                },
                error: function(data) {
                    alert('error');
                }

            });
            //var json=require("https://www.alphavantage.co/query?func;on=TIME_SERIES_DAILY&symbol=MSFT&apikey=ZH0JISPRJNEON69U");
            //file = JSON.parse(json);
            //console.log(file);
        });
        $('#trashaapl').click(function() {
            $('.aapl_fav').remove();
            alert("hi");
        });
        $("#clear").click(function() {
            $('#chart').css("display", "none");
            $("#favorite-chart").css("display", "block");
        });
        $('#tostock').click(function() {
            $("#favorite-chart").css("display", "none");
            $("#chart").css("display", "block");
        });
        $('#tofav').click(function() {
            $("#chart").css("display", "none");
            $("#favorite-chart").css("display", "block");

        });
    });

    function creategraph(ind) {
        var symbol = ind["symbol"];
        var title = ind["title"];
        var date = ind["date"];
        var ini = ind["indicator"].match(/\((\S*)\)/)[1];
        var sub_name = ind["sub_name"];
        var ind_stat = ind["stat"];
        var statis = ind_stat[sub_name[0]];
        var index = ini + "_con";
        var chart = Highcharts.chart(index, {

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
                align: 'right',
                verticalAlign: 'middle'
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
    function createprice(ind){
            var symbol = ind["symbol"];
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
            var chart1 = Highcharts.chart("price_con", {
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
                    align: 'right',
                    verticalAlign: 'middle'
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
    /**
     * Create the chart when all data is loaded
     * @returns {undefined}
     */
    function createChart(ind) {
        var symbol = ind.symbol;
        var date = ind.date;
        var price =ind.price;
        var data = [];
        for(i=0;i< date.length;i++){
            date[i]=parseInt(date[i]+'000');
            price[i]=parseFloat(parseFloat(price[i]).toFixed(2));
            data[i]=[date[i],price[i]];
        }
        Highcharts.stockChart('history', {


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