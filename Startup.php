<?php
ini_set('display_startup_errors', '1');
function time_elapsed_B($secs){
    $bit = array(
        ' hour'        => $secs / 3600 % 24,
        ' minute'    => $secs / 60 % 60,
        ' second'    => $secs % 60
        );
        
    foreach($bit as $k => $v){
        if($v > 1)$ret[] = $v;
        if($v == 1)$ret[] = $v;
        }
    if ($bit[' minute'] > 0) {
        array_splice($ret, count($ret)-1, 0, '0');
    } else {
    array_splice($ret, count($ret)-1, 0, '');
    }
    $ret[] = '';
    
    return join(':', $ret);
}
$now = new DateTime();
if ($_SERVER['REMOTE_ADDR'] != "75.51.150.193") {
file_put_contents("usagelog.log", "<p>" . $_SERVER['REMOTE_ADDR'] . "</p><p>" . time_elapsed_B($now->getTimestamp()) . "</p><br>\n", FILE_APPEND);
} else {
file_put_contents("usagelog.log", "<p><small><small>" . $_SERVER['REMOTE_ADDR'] . "</p><p>" .  time_elapsed_B($now->getTimestamp()) . "</small></small></p><br>\n", FILE_APPEND);
}
$dl = file_get_contents("usagesum.log");
file_put_contents("usagesum.log", (string)(intval($dl) + 1));
$nowtime = time();
file_put_contents("lastused.log", $nowtime);

$motd = file_get_contents("motd");

$dl = intval($dl) + 1;

if (round($dl - floor($dl / 10) * 10) == 1) {
    $nend = "st";
} elseif (round($dl - floor($dl / 10) * 10) == 2) {
    $nend = "nd";
} else{
    $nend = "rd";
}

if ($_SERVER['REMOTE_ADDR'] == "75.51.150.193") {
    $motd = "User: " . (string)($dl) . "; Last User: " . file_get_contents("lastuser.log") . "\nMOTD: " . $motd = file_get_contents("motd");
} else {
    $motd = file_get_contents("motd");
}

echo '{"Updates":{"Stable":{' . file_get_contents('CurrentVersion') . ',"Build":13,"File":"\/Update\/win32-sse2.ptu"},"Beta_Unused":{"Major":90,"Minor":1,"Build":320,"File":"\/Download\/Builds\/Build-320\/-.ptu"},"Snapshot":{"Major":83,"Minor":3,"Build":208,"Snapshot":1346881831,"File":"\/Download\/Builds\/TPTPP\/-.ptu"}},"Notifications":[],"Session":true,"MessageOfTheDay":"' . $motd . '"} ';
?>