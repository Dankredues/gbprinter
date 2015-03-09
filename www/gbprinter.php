<?php
include "nav.php";

$r= $_GET['r'];
$r= strtoupper($r);
$n= $_GET['n'];
$m= $_GET['m'];
$n= strtoupper($n);
echo "$n : $r <br> ";

function fill($r,$max)
{
	$ret=$r;
	$c=$max-strlen($r);
	for($i=0;$i<$c;$i++)
	{
		$ret=$ret."X";
	}
	return $ret;
}

echo exec('sudo /home/pi/gbprint "'.$n.'  SCHREIBT                                        " NEG > /tmp/log.txt');
echo exec('sudo /home/pi/gbprint "                                                        " POS > /tmp/log.txt');

	echo strlen($r)."<br>";
	for($i=0; $i<strlen($r);$i=$i+20)
	{
		echo "i=". $i .", i+1= ".($i+20) ."<br>";
		$r2= substr($r,$i,20 );
		$r2= fill($r2,20);
		echo "$r2 #<br>";
		echo exec('sudo /home/pi/gbprint "'.$r2.'                        " POS > /tmp/log.txt');
	}
echo exec('sudo /home/pi/gbprint "NACHR ENDE                                           " NEG > /tmp/log.txt');

?>
