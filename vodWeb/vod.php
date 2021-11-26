<?php
require("../libs/vodlib.php");
Authorize($hash,$user);
require("header.php");

include($_GET["action"]);

echo '<br><br><br>';
require("footer.php");
?>