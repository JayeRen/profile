<!DOCTYPE html>
<html>
<head>
    <style type="text/css">
        .div{
            margin: 0 auto;
            text-align: center;
        }
    </style>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
<!--   jquery-->
    <script src="http://code.jquery.com/jquery-3.2.1.js" integrity="sha256-DZAnKJ/6XZ9si04Hgrsxu/8s717jcIzLy3oi35EouyE=" crossorigin="anonymous"></script>
<!--    ajax-->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.3/umd/popper.min.js" integrity="sha384-vFJXuSJphROIrBnz7yo7oB41mKfc8JzQZiCq4NCceLEaO4IHwicKwpJf9c9IpFgh" crossorigin="anonymous"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js" integrity="sha384-alpBpkh1PFOepccYVYDB4do5UnbKysX5WZXm3XxPqe5iKTfUKjNkCk9SaVuEZflJ" crossorigin="anonymous"></script>
    </head>
    <body>
    
        <div><input type=text name='symbol' id='symbol'></div>
        <button type="submit" id="search">search</button>
        
    </body>
</html>

<script>
$(document).ready(function() {
    $("#search").click(function() {
        $symbol=$('#symbol').val();
        $('#symbol').text($symbol);
        $.ajax({
            url: "phplink/data/auto.php",
            async: true,
            contentType: "application/json; charset=utf-8",
            data: {
                symbol: JSON.stringify($symbol)
            },
            dataType: "json",
            cache: true,
            success: function(data) {
                alert(data);
            },
            error: function() {
                alert("error");
            }
        });
    });  
});
</script>