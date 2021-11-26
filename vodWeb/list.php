

<style>
<!--
#foldheader{cursor:hand ; font-weight:bold ;
list-style-image:  url(images/fold.gif)}
#foldinglist{list-style-image:  url(images/list.gif)}
//-->
</style>
<script language="JavaScript1.2">
<!--

var head="display:''"
img1=new Image()
img1.src="images/fold.gif"
img2=new Image()
img2.src="images/open.gif"

function change(){
   if(!document.all)
      return
   if (event.srcElement.id=="foldheader") {
      var srcIndex = event.srcElement.sourceIndex
      var nested = document.all[srcIndex+1]
      if (nested.style.display=="none") {
         nested.style.display=''
         event.srcElement.style.listStyleImage="url(images/open.gif)"
      }
      else {
         nested.style.display="none"
         event.srcElement.style.listStyleImage="url(images/fold.gif)"
      }
   }
}

document.onclick=change

//-->
</script>
<?php
//<li><a href="http://page1.htm">Page1</a></li>

$link=DBconnect();

echo'<table border=0 align="center"><tr><td><li id="foldheader">Eastern Mediterranean University</li>
<ul id="foldinglist" style="display:none" style=&{head};>';
	
$result1 = mysql_query("SELECT id,name FROM faculty order by name", $link);
if (!$result1) die("query failed: ".mysql_error());
while($row1 = mysql_fetch_array($result1)){
echo'<li id="foldheader">',$row1["name"],'</li> <ul id="foldinglist" style="display:none" style=&{head};>';
	
	$result2 = mysql_query("SELECT id,name FROM department WHERE facid='$row1[id]' order by name", $link);
	if (!$result2) die("query failed: ".mysql_error());
	while($row2 = mysql_fetch_array($result2)){
	echo'<li id="foldheader">',$row2["name"],'</li> <ul id="foldinglist" style="display:none" style=&{head};>';
	
		$result3 = mysql_query("SELECT id,name FROM course WHERE depid='$row2[id]' order by name", $link);
		if (!$result3) die("query failed: ".mysql_error());
		while($row3 = mysql_fetch_array($result3)){
		echo'<li id="foldheader">',$row3["name"],'</li> <ul id="foldinglist" style="display:none" style=&{head};>';
	
			$result4 = mysql_query("SELECT id,name FROM chapter WHERE csid='$row3[id]'", $link);
			if (!$result4) die("query failed: ".mysql_error());
			while($row4 = mysql_fetch_array($result4)){
			echo'<li id="foldheader">',$row4["name"],'</li> <ul id="foldinglist" style="display:none" style=&{head};>';
	
				$result5 = mysql_query("SELECT id,name FROM project WHERE chid='$row4[id]'", $link);
				if (!$result5) die("query failed: ".mysql_error());
				while($row5 = mysql_fetch_array($result5)){
				echo'<li><a href="vod.php?action=projects/project.php&prid=',$row5["id"],'">',$row5["name"],'</a></li> <ul id="foldinglist" style="display:none" style=&{head};>';
				
	

				echo '</ul>';
				}

			echo '</ul>';
			}
	

		echo '</ul>';
		}
	

	echo '</ul>';
	}

echo '</ul>';
}


echo '</ul>';



/*		
		<li id="foldheader">Electrical & Electronic Engineering</li>
		<ul id="foldinglist" style="display:none" style=&{head};>
		
			<li id="foldheader">EEE474 - Software Engineering</li>
			<ul id="foldinglist" style="display:none" style=&{head};>
			
				<li id="foldheader">Chapter 1 - Introduction to Software Engineering</li>
				<ul id="foldinglist" style="display:none" style=&{head};>
			
					<li><a href="http://page1.htm">Project 1</a></li>
				
				</ul>
			</ul>
		</ul>
	</ul>
</td></tr></table>
';*/

DBconnect($link);
?>



