<?php
$idurl = $_GET['id'];
$post_7 = get_post($idurl); 
$title = $post_7->post_title;
$content = $post_7->post_content;
echo '<head>';
echo '<link rel="stylesheet" type="text/css" href="http://www.conseilsencuisine.fr/style-impression.css">';
echo '<script language="javascript">';
echo 'window.print()';
echo '</script> ';
echo '<title>  '.$title.' | Conseils En Cuisine !</title>';
echo '<link rel="icon" type="image/png" href="http://www.conseilsencuisine.fr/images/favicon1.ico" />';
echo '</head>';
echo '<body>';
echo '<div style="width:21cm;margin-left: auto;margin-right: auto;">';
echo '<div id="title"><div style="position:relative;float:left;"><img src="http://www.conseilsencuisine.fr/images/favicon1.ico" alt="" width="16px" ></div><div style="vertical-align:middle;position:relative;float:left;"><span style="color:#1C68D0;font-size:10pt;margin-left:5px"><b>Conseils en Cuisine !</b></span></div><div style="vertical-align:middle;position:relative;float:right;"><span style="color:#1C68D0;font-size:10pt"><b>http://www.conseilsencuisine.fr/recette-'.$idurl.'</b></span></div></div>';
echo '<div id="lines"><span style="color:#CDCDCD;"><b>- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -</b></span></div>';
echo '<div id="title" style="text-align:left;"><h1>'.$title.'</h1></div>';
//Image :
$marqueurDebutImg = "<photo>";
$debutImg = strpos( $content, $marqueurDebutImg ) + strlen( $marqueurDebutImg );
$marqueurFinImg = "</photo>";
$finImg = strpos( $content, $marqueurFinImg );
$leImg = substr( $content, $debutImg, $finImg - $debutImg );
echo "<div id='img'>".$leImg;
//Temps :
$marqueurDebutTps = "<temps>";
$debutTps = strpos( $content, $marqueurDebutTps ) + strlen( $marqueurDebutTps );
$marqueurFinTps = "</temps>";
$finTps = strpos( $content, $marqueurFinTps );
$leTps = substr( $content, $debutTps, $finTps - $debutTps );
echo $leTps."</div>";
//Ingredients :
$marqueurDebutIngr = "<ingredients>";
$debutIngr = strpos( $content, $marqueurDebutIngr ) + strlen( $marqueurDebutIngr );
$marqueurFinIngr = "</ingredients>";
$finIngr = strpos( $content, $marqueurFinIngr );
$leIngr = substr( $content, $debutIngr, $finIngr - $debutIngr );
echo "<div id='ingr'>".$leIngr."</div>";
//Preparation :
$marqueurDebutPrep = "<preparation>";
$debutPrep = strpos( $content, $marqueurDebutPrep ) + strlen( $marqueurDebutPrep );
$marqueurFinPrep = "</preparation>";
$finPrep = strpos( $content, $marqueurFinPrep );
$lePrep = substr( $content, $debutPrep, $finPrep - $debutPrep );
echo "<div id='prepcons'>".$lePrep."</div>";
//conseils :
$marqueurDebutCons = "<conseils>";
$debutCons = strpos( $content, $marqueurDebutCons ) + strlen( $marqueurDebutCons );
$marqueurFinCons = "</conseils>";
$finCons = strpos( $content, $marqueurFinCons );
$leCons = substr( $content, $debutCons, $finCons - $debutCons );
echo "<div id='prepcons'>".$leCons."</div>";
?> 
</div>
</body>
