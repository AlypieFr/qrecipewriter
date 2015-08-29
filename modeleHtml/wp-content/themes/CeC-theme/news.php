<head>
<link rel="stylesheet" type="text/css" href="http://www.conseilsencuisine.fr/style-impression.css">
</head>
<body>
<?php
$post_7 = get_post(1478); 
$title = $post_7->post_title;
$content = $post_7->post_content;
echo '<div style="width:21cm;margin-left: auto;margin-right: auto;">';
echo '<h1>'.$title.'</h1>';
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
echo '</div>';
?> 
</body>
