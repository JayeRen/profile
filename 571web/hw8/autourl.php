<html lang="en" >
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
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
<script
  src="http://code.jquery.com/jquery-3.2.1.js"
  integrity="sha256-DZAnKJ/6XZ9si04Hgrsxu/8s717jcIzLy3oi35EouyE="
  crossorigin="anonymous"></script>
    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css" integrity="sha384-PsH8R72JQ3SOdhVi3uxftmaW6Vc51MKb0q5P2rRUpPvrszuE4W1povHYgTpBfshb" crossorigin="anonymous">
    <link rel="stylesheet" href="font-awesome/css/font-awesome.min.css">
    
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
<body ng-app="myApp" ng-cloak>
  <!--
    Your HTML content here
  -->  

<div class="row">
<form id="needs-validation" ng-submit="$event.preventDefault()" novalidate> 
    <label for="SymbolInput" class="col  col-form-label">Enter Stock Ticker Symbol:</label>
    
    <div class="col"  ng-controller="SearchCtrl as ctrl" layout="column" ng-cloak>
      <md-content class="md-padding">
            <md-autocomplete   
              ng-disabled="ctrl.isDisabled"
              md-selected-item="ctrl.selectedItem"
              md-search-text="ctrl.searchText"
              md-items="item in ctrl.querySearch(ctrl.searchText)"
              md-item-text="item.display"
              md-min-length="0"
              placeholder="e.g.AAPL" id="symbol" required>
            <input type="text" class="form-control" required>
            <md-item-template>
              <span md-highlight-text="ctrl.searchText" md-highlight-flags="^i" id="SymbolInput">{{item.display}}</span>
            </md-item-template>
          </md-autocomplete>
          </md-content>
          </div>
          <div class="col">
            <button type="submit" class="btn" id="search">Get Quote<i class="fa fa-search" aria-hidden="true"></i></button>
              <button type="reset" class="btn btn-secondary" id="clear"><i class="fa fa-refresh" aria-hidden="true"></i>
            Clear</button>
            </div>
          
  
</form>
</div>


  
  <!-- Your application bootstrap  -->
  <script type="text/javascript">    
    /**
     * You must include the dependency on 'ngMaterial' 
     */
//    angular.module('BlankApp', ['ngMaterial']);
//    
      
      angular
      .module('myApp', ['ngMaterial'])
      .controller('SearchCtrl', SearchCtrl);

  function SearchCtrl ($timeout, $q, $log, $http) {
    var self = this;

    self.simulateQuery = false;
    self.isDisabled    = false;

    // list of `state` value/display objects
    //main query-----result of all symbol
    self.querySearch   = querySearch;
//    self.selectedItemChange = selectedItemChange;
//    self.searchTextChange   = searchTextChange;

    self.newState = newState;

    function newState(state) {
      alert("Sorry! You'll need to create a Constitution for " + state + " first!");
    }

    // ******************************
    // Internal methods
    // ******************************
    /**
     * Search for states... use $timeout to simulate
     * remote dataservice call.
     */
    function querySearch (query) {
        //$url="http://dev.markitondemand.com/MODApis/Api/v2/Lookup/json?input="+query;
        $http.get("auto.json")
             .then(function (response) {
               $tag=[];
                $data=response.data;
               for ($i=0;$i<$data.length;$i++) {
                    $txt = $data[$i].Symbol + " - " + $data[$i].Name + ' (' + $data[$i].Exchange + ')';
                    $tag.push($txt);
//                   alert($tag);
                }
               //alert($tag);
               self.states = $tag.map( function (state) {
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
        $timeout(function () { deferred.resolve( results ); }, Math.random() * 1000, false);
        return deferred.promise;
      } else {
        return results;
      }
    }
//    function searchTextChange(text) {
//      $log.info('Text changed to ' + text);
//    }
//    function selectedItemChange(item) {
//      $log.info('Item changed to ' + JSON.stringify(item));
//    }
      
    /**
     * Build `states` list of key/value pairs
     "http://dev.markitondemand.com/MODApis/Api/v2/Lookup/json?input=Apple"
     */
//    function loadAll() {
//
//    }
    /**
     * Create filter function for a query string
     */
//    function createFilterFor(query) {
//      var lowercaseQuery = angular.lowercase(query);
//
//      return function filterFn(state) {
//        return (state.value.indexOf(lowercaseQuery) === 0);
//      };
//    }
  }

//      var form = document.getElementById('needs-validation');
//    $('#search').click(function(event){
//        if (form.checkValidity() === false) {
//        event.preventDefault();
//        event.stopPropagation();
//      }
//      form.classList.add('was-validated');
//    })
      
         // Your code goes here.
      
    $(document).ready(function() {
        var form = document.getElementById('needs-validation');
        $("#search").click(function(event) {
            if (form.checkValidity() === false) {
                event.preventDefault();
                event.stopPropagation();
            }
            form.classList.add('was-validated');
            $symbol =$('#symbol')->[md-selected-item];
            //$symbol=$symbol.split(' ')[0];
        console.log($symbol.toString());
            $.ajax({
                //type: "GET",
                url: "/hw8/search.php",
                async: true,
                contentType: "application/json; charset=utf-8",
                data: {
                    input: $symbol
                },
                dataType: "json",
                cache: true,
                success: function(data) {
                    //stock_info part
                    //alert(data);
                    //var data = JSON.stringify(data);
//                    $("#SymbolInput").text(data.sts);
//                    $('#sts_val').text(data.sts);
//                    $('#last_price_val').text(data.lastprice);
//                    $('#change_val').html(data.change + "<img src=' " + data.image + "' height=10 width=10>");
//                    $('#timestamp_val').text(data.timestamp);
//                    $('#open_val').text(data.open);
//                    $('#close_val').text(data.close);
//                    $('#days_range_val').text(data.daysrange);
//                    $('#volume_val').text(data.volume);
//                    $('#chart').css("display", "block");
//                    $('#favorite-chart').css("display", "none");
//                    //localstorage
//                    $temp = data.change + "<img src=' " + data.image + "' height=10 width=10>";
//                    $fav_txt = "<td>" + data.sts + "</td><td>" + data.lastprice + "</td><td>" + $temp + "</td><td>" + data.volume + "</td><td><button type='button' class='btn btn-default btn-lg'  id='trash" + data.sts + "'><i class='fa fa-trash' aria-hidden='true'></i></button>" + "</td>";
//
//                    //graph
//                    //$stat = data.ind;
//                    //creategraph(data.sma);
//                    //(recover later)
//                    //                    creategraph(data.ema);
//                    //                    creategraph(data.stoch);
//                    //                    creategraph(data.rsi);
//                    //                    creategraph(data.adx);
//                    //                    creategraph(data.cci);
//                    //                    creategraph(data.bbands);
//                    //                    creategraph(data.macd);
//                    //                    //news
//                    $('#news_table').html(data.news1 + data.news2 + data.news3 + data.news4 + data.news5);
//                    //favorite function:only exists when symbol exists
//                    $fav = 0;
//                    $('#favorite').click(function() {
//                        if ($fav == 0) {
//                            $('#fav-off').css("display", "none");
//                            $('#fav-on').css("display", "block");
//                            localStorage.setItem("fav_" + data.sts, $fav_txt);
//
//                            $("#favorite-table").append("<tr id='" + data.sts + "_fav'>" + localStorage.getItem("fav_" + data.sts) + "</tr>");
//                            $fav = 1;
//                        } else if ($fav == 1) {
//                            $('#fav-on').css("display", "none");
//                            $('#fav-off').css("display", "block");
//                            localStorage.removeItem("fav_" + data.sts);
//                            $r_tag = '#' + data.sts + "_fav";
//                            $($r_tag).remove();
//                            $fav = 0;
//                        }
//                    });
//                    $('#fb').click(function() {
//                        FB.ui({
//                            app_id: '696698673858174',
//                            method: 'feed',
//                            link: 'https://developers.facebook.com/docs/'
//                        }, function(response) {
//                            if (response && !response.error_message) {
//                                console.log("success");
//                                //succeed 
//                            } else {
//                                //fail 
//                                console.log("error");
//                            }
//                        });
//                    });
                    //        $("#favorite-table").html($ft);
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
  </script>
  
</body>
</html>
