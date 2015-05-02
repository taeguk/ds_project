<!DOCTYPE html>
<html lang="ko">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>도서관</title>

	<!-- Bootstrap -->
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.0/css/bootstrap.min.css">

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>
	<style>
		.progress {
			margin-bottom:0px;
		}
		@media screen and (max-width: 767px) {
			.ratio {
				min-width: 300px;
			}
		}
	</style>
  <body>
	<div class = "container">
		<div class = "row">
			<div class = "col-lg-12">
				<div class = "page-header">
					<h1>랩실 미만 잡</h1>
				<div>
			</div>
		</div>
		<div class = "row">
			<div class = "col-lg-12">
				<div class = "table-responsive">
				<table class = "table table-bordered" style="width:100%;">
					<thead>
						<tr>
							<th style="width:25;">wh</th>
							<th style="width:10%;">to</th>
							<th style="width:10%;">us</th>
							<th style="width:10%;">re</th>
							<th style="width:45%;" class="ratio">ra</th>
						</tr>
					</thead>
					<tbody>
<?php
$fp = fopen("./zzc001.json","r") or die("망함");
$j = fread($fp, filesize("./zzc001.json"));
fclose($fp);
$a = json_decode($j);
$n = count($a);
for($i=0; $i<$n; $i++) {
	echo "<tr>";
	echo "<td>".$a[$i]->name."</td>";
	echo "<td>".$a[$i]->total."</td>";
	echo "<td>".$a[$i]->used."</td>";
	echo "<td>".$a[$i]->remain."</td>";
	//echo "<td>".$a[$i]->ratio."</td>";
	$r = $a[$i]->ratio;
	echo "<td>";
	echo "<div class=\"progress\">
	  <div class=\"progress-bar progress-bar-striped active\" role=\"progressbar\" aria-valuenow=\"$r\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"width: $r%;\">
	      $r%
		    </div>
		</div>";
	echo "</td>";
	echo "</tr>";

}

//echo $j;
?>
					</tbody>
				</table>
				</div>
			</div>
		</div>
	</div>


    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>

	<!-- Include all compiled plugins (below), or include individual files as needed -->
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.0/js/bootstrap.min.js"></script>

  </body>
</html>
