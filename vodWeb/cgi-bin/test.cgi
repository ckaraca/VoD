#!/usr/bin/perl

# Solution Scripts Test Script
# http://solutionscripts.com

# Please make sure all items in this script work before purchasing a license

### Install ###
#
# Make sure the first line is the correct location to perl
# (may be #!/usr/local/bin/perl on some servers)
#
# Upload to the server and chmod 755.
# Call test.cgi via a browser and follow instructions on the screen
#


########
#
# Used only for testing piping email to cgi script
# Alias-Mail Professional and XtremeForum (optional)
#
########


$config_email = 'emailaddress@example.net';
# Email to send debugging information to.
# Should not be the same address that will be piping email to the script, as this will cause a nasty loop

$config_prog = '/usr/sbin/sendmail';
# Program used to send out emails.
# The local of sendmail or qmail's qmail-inject program

######### EDIT NO MORE ###########

unless ($ENV{'REMOTE_ADDR'}) { ## EMAIL PIPING ##
    &pipe_email;
}    

$INPUT = &get_input;

push(@{$nodes},{
	NAME => 'user',
	TEXT => "User Access",
});

push(@{$nodes},{
	NAME => 'aliasmail',
	TEXT => "Alias-Mail Professional",
});

push(@{$nodes},{
	NAME => 'homefree',
	TEXT => "Home Free Professional",
});

push(@{$nodes},{
	NAME => 'forum',
	TEXT => "XtremeForum",
});

&header;

if ($INPUT->{'action'} eq "db_check") {
    &db_check;
}


print qq~
<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk" width="100%">
<TR><TD><TABLE cellpadding=10 cellspacing=1 border=0 width="100%">
<TR><TD CLASS="tblmain">
<B>W</B>elcome to the Solution Scripts Test Script, this short perl script will help you prepare your server for an easy install of our programs.
<BR><BR>
You should make sure you pass all required options before purchasing a license to a Solution Scripts program.

</TD></TR></TABLE>
</TD></TR></TABLE>    

</TD></TR>
<TR><TD CLASS="tblmain">    

<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk" width="100%">
<TR><TD><TABLE cellpadding=10 cellspacing=1 border=0 width="100%">
<TR><TD CLASS="tblmain">
<FONT CLASS="lrg"><B>Checking for required perl modules for:</B></FONT><BR><BR>
<B>
~;

my $mod_form;

$INPUT->{'check_user'} = 1;
my $node_hash;

foreach my $node (@{$nodes}) {
    $node_hash->{$node->{'NAME'}} = $node->{'TEXT'};
    
    print "<A HREF=\"$node->{'URL'}\" TARGET=\"_blank\">$node->{'TEXT'}</A><BR>" if ($INPUT->{'check_' . $node->{'NAME'}});

    $mod_form .= qq~
    <TR><TD CLASS="tblmain">
    <INPUT TYPE="CHECKBOX" NAME="check_$node->{'NAME'}"
    ~;
    $mod_form .= "CHECKED" if $INPUT->{'check_' . $node->{'NAME'}};
    $mod_form .= qq~
    > $node->{'TEXT'} &nbsp;
    </TD></TR>
    ~;
}
    


print qq~
</B>
<BR>
<B>Note:</B> all User Access modules are required to use any of the other programs/nodes
<BR><BR>
A perl module is a block of code that performs a specific function. In using modules, 
a developer does not need to reinvent the wheel (ie recode) to to do an action, 
but just calls the the module and thus the prewritten code. 
Perl comes with several hundred standard modules and the perl community has several 
thousand more that do not come bundled with perl. Solution Scripts has coded our 
programs to use bundled modules when available and tried not to use many non-standard modules, 
although to make our time and code more effient and clean, we have used some non-standard modules. 
This script lists the required modules and determines if the module is present on this server or not.
For information on attaining the required modules and installing them, visit: 
<A HREF="http://faq.solutionscripts.com/manual/user/install/misc/modules.html">http://faq.solutionscripts.com/manual/user/install/misc/modules.html</A>
<BR><BR>
<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk">
<TR><TD><TABLE cellpadding=5 cellspacing=1 border=0 width="100%">
~;

my @mod_array = qw(DBI.pm DBD/mysql.pm);
my $mods;

push(@{$mods},{
	NAME => 'DBI.pm',
	TEXT => "DBI",
	DESC => "Database connectivity",
    MANUAL => '',
    NODE => 'user',
    DOWNLOAD => 'http://search.cpan.org/search?dist=DBI'
});

push(@{$mods},{
	NAME => 'DBD/mysql.pm',
	TEXT => "DBI::mysql - Msql-Mysql-modules",
	DESC => "Database connectivity",
    MANUAL => '',
    NODE => 'user',
    DOWNLOAD => 'http://search.cpan.org/search?dist=Msql-Mysql-modules'
});

push(@{$mods},{
	NAME => 'Data/Dumper.pm',
	TEXT => "Data::Dumper",
	DESC => "Database connectivity",
    MANUAL => '',
    NODE => 'user',
    DOWNLOAD => 'http://search.cpan.org/search?dist=Data-Dumper'
});

push(@{$mods},{
	NAME => 'Data/ShowTable.pm',
	TEXT => "Data::ShowTable",
	DESC => "Database connectivity",
    MANUAL => '',
    NODE => 'user',
    DOWNLOAD => 'http://search.cpan.org/search?dist=Data-ShowTable'
});

push(@{$mods},{
	NAME => 'MD5.pm',
	TEXT => "MD5",
	DESC => "Authentication / Security",
    MANUAL => '',
    NODE => 'user',
    DOWNLOAD => 'http://search.cpan.org/search?dist=MD5'
});

## ALIAS-MAIL ##

push(@{$mods},{
	NAME => 'Text/Wrap.pm',
	TEXT => "Text::Wrap",
	DESC => "Reply quoting",
    MANUAL => '',
    NODE => 'aliasmail',
    DOWNLOAD => 'http://search.cpan.org/doc/MUIR/Text-Tabs+Wrap-2001.0131/lib/Text/Wrap.pm'
});

push(@{$mods},{
	NAME => 'MIME/Tools.pm',
	TEXT => "MIME::Tools",
	DESC => "Encoding/Decoding mail attachments",
    MANUAL => '',
    NODE => 'aliasmail',
    DOWNLOAD => 'http://search.cpan.org/search?dist=MIME-tools'
});

push(@{$mods},{
	NAME => 'IO/Scalar.pm',
	TEXT => "IO::Scaler - IO::Stringy",
	DESC => "Encoding/Decoding mail attachments",
    MANUAL => '',
    NODE => 'aliasmail',
    DOWNLOAD => 'http://search.cpan.org/search?dist=IO-stringy'
});

push(@{$mods},{
	NAME => 'MIME/Base64.pm',
	TEXT => "MIME::Base64",
	DESC => "Encoding/Decoding mail attachments",
    MANUAL => '',
    NODE => 'aliasmail',
    DOWNLOAD => 'http://search.cpan.org/search?dist=MIME-Base64'
});

push(@{$mods},{
	NAME => 'Mail/Header.pm',
	TEXT => "Mail::Header - MailTools",
	DESC => "Encoding/Decoding mail attachments",
    MANUAL => '',
    NODE => 'aliasmail',
    DOWNLOAD => 'http://search.cpan.org/search?dist=MailTools'
});

push(@{$mods},{
	NAME => 'Time/ParseDate.pm',
	TEXT => "Time::ParseDate - Time-modules",
	DESC => "Converting incoming email dates",
    MANUAL => '',
    NODE => 'aliasmail',
    DOWNLOAD => 'http://search.cpan.org/search?dist=Time-modules'
});

push(@{$mods},{
	NAME => 'Net/POP3.pm',
	TEXT => "Net::POP3 - LibNet Bundle",
	DESC => "Retrieving external mail (optional)",
    MANUAL => '',
    NODE => 'aliasmail',
    DOWNLOAD => 'http://search.cpan.org/search?dist=libnet'
});


##### HOME FREE #####
push(@{$mods},{
	NAME => 'URI.pm',
	TEXT => "URI",
	DESC => "Retrieving web pages from other servers (optional)",
    MANUAL => '',
    NODE => 'homefree',
    DOWNLOAD => 'http://search.cpan.org/search?dist=URI'
});

push(@{$mods},{
	NAME => 'LWP/UserAgent.pm',
	TEXT => "LWP::UserAgent - libwww bundle",
	DESC => "Retrieving web pages from other servers (optional)",
    MANUAL => '',
    NODE => 'homefree',
    DOWNLOAD => 'http://search.cpan.org/search?dist=libwww-perl'
});

##### XTREMEFORUM #####
push(@{$mods},{
	NAME => 'Text/Wrap.pm',
	TEXT => "Text::Wrap",
	DESC => "Reply quoting",
    MANUAL => '',
    NODE => 'forum',
    DOWNLOAD => 'http://search.cpan.org/doc/MUIR/Text-Tabs+Wrap-2001.0131/lib/Text/Wrap.pm'
});


######
foreach my $mod (@{$mods}) {
    
    next unless ($INPUT->{'check_' . $mod->{'NODE'}});
    
    print qq~
    <TR><TD CLASS="tblhdr">
    <B>$mod->{'TEXT'}</B> - $mod->{'DESC'}
    <BR><SMALL>$node_hash->{$mod->{'NODE'}}</SMALL>
    </TD></TR>
    <TR><TD CLASS="tblmain">
    ~;
    
    eval { require $mod->{'NAME'}; };
        
    if ($@) {
        print qq~
        <FONT COLOR=RED><B>FAILED</B></FONT> - $@
        <BR><BR>
        Download: <A HREF="$mod->{'DOWNLOAD'}" TARGET="_blank">$mod->{'DOWNLOAD'}</A>
        ~;
    }
    else {
        print qq~
        <FONT COLOR=GREEN><B>Passed</B></FONT>
        ~;
    }
    
    
    print qq~
        </TD></TR>
    ~;

}

print qq~
</TABLE>
</TD></TR></TABLE>  

<FORM METHOD=POST ACTION="test.cgi">
<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk">
<TR><TD><TABLE cellpadding=5 cellspacing=1 border=0 width="100%">
<TR><TD CLASS="tblhdr">
Refresh perl module check using the following programs/nodes:
</TD></TR>
$mod_form
<TR><TD CLASS="tblhdr">
<INPUT TYPE=SUBMIT>
</TD></TR></TABLE>
</TD></TR></TABLE>
</FORM>


</TD></TR></TABLE>
</TD></TR></TABLE>   


</TD></TR>
<TR><TD CLASS="tblmain">    

<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk" width="100%">
<TR><TD><TABLE cellpadding=10 cellspacing=1 border=0 width="100%">
<TR><TD CLASS="tblmain">
<FONT CLASS="lrg"><B>Database connectivity:</B></FONT>
<BR><BR>
This section will test if a perl script can properly access a MySQL database. For this to work, you must have installed all modules above that deal with "database connectivity", have a running MySQL server, and have a working MySQL username and password.
<BR><BR>

<FORM METHOD=POST ACTION="test.cgi">
<INPUT TYPE=HIDDEN NAME="action" VALUE="db_check">
<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk">
<TR><TD><TABLE cellpadding=5 cellspacing=1 border=0 width="100%">
<TR><TD CLASS="tblhdr" COLSPAN=2>
Enter MySQL related information
</TD></TR>
<TR>
<TD CLASS="tblmain">MySQL server: (usually localhost)</TD>
<TD CLASS="tblmain"><INPUT TYPE="TEXT" NAME="domain" VALUE="localhost" SIZE=20></TD>
</TR>
<TR>
<TD CLASS="tblmain">Username:</TD>
<TD CLASS="tblmain"><INPUT TYPE="TEXT" NAME="usr" VALUE="" SIZE=20></TD>
</TR>
<TR>
<TD CLASS="tblmain">Password:</TD>
<TD CLASS="tblmain"><INPUT TYPE="TEXT" NAME="pwd" VALUE="" SIZE=20></TD>
</TR>
<TR>
<TD CLASS="tblmain">Database Name:</TD>
<TD CLASS="tblmain"><INPUT TYPE="TEXT" NAME="db" VALUE="" SIZE=20></TD>
</TR>
<TR><TD CLASS="tblhdr" COLSPAN=2 ALIGN=CENTER>
<INPUT TYPE=SUBMIT>
</TD></TR></TABLE>
</TD></TR></TABLE>
</FORM>

</TD></TR></TABLE>
</TD></TR></TABLE>   

</TD></TR>
<TR><TD CLASS="tblmain">    

<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk" width="100%">
<TR><TD><TABLE cellpadding=10 cellspacing=1 border=0 width="100%">
<TR><TD CLASS="tblmain">
<FONT CLASS="lrg"><B>Incoming Emails:</B></FONT>
<BR><BR>
<B>(Alias-Mail Professional and XtremeForum only)</B> (XtremeForum is optional as well)
<BR><BR>
For Alias-Mail to work properly, all email for domains for which Alias-Mail will handle email need to be configured to pipe all incoming email to the cgi script.
This allows Alias-Mail to distribute the mail to users mailboxes properly and keep track of things like how much space a user is using.
<BR><BR>
XtremeForum differs in that it needs to have email from only one address piped to the script. This is for the feature that allows people to respond to posts via email and is totally optional.
<BR><BR>
What we mean when we say, "pipe to a script" is that when the email is received by the server, instead of being place in a folder as a text file for later pickup as is normal, we need the incoming email to execute the cgi script as the email reaches the server, 
just like a page view to a web site executes the script on every hit by a user. Once the script is executed, the program then has the full text of the email in memory and can then decide what actions need to be done. (ie email saved and users info update, or in the case of XtremeForum, a new message is posted to the forum).
<BR><BR>
To do this piping, we intercept the email after it reaches the server and at the point the "mail delivery agent" is looking for who on the server the email should be delivered to.
How this is done depends on the email software running on the server. Most servers either have, Sendmail, a combination of Sendmail and Procmail, or Qmail. Either of these options will work, although with Alias-Mail, just plain Sendmail without Procmail is not recommended and will cause some email not to be delivered.
<BR><BR>
Alias-Mail also needs to have a catchall set, this will forward all email for a domain to one user, where it then can be piped to the cgi script. 
Details on how to set up this catchall can be found here:
<A HREF="http://faq.solutionscripts.com/manual/aliasmail/install/sendmail.html" target="_blank">Sendmail</A> or
<A HREF="http://faq.solutionscripts.com/manual/aliasmail/install/qmail.html" target="_blank">Qmail</A>
<BR><BR>
Documentation on how to set up email piping:
<A HREF="http://faq.solutionscripts.com/manual/aliasmail/install/forward.html" target="_blank">Sendmail via .forward</A>, 
<A HREF="http://faq.solutionscripts.com/manual/aliasmail/install/procmail.html" target="_blank">Sendmail w/ procmail</A>, or
<A HREF="http://faq.solutionscripts.com/manual/aliasmail/install/qmail.html" target="_blank">Qmail</A>
<BR><BR>
For testing purposes, email software should be configured to pipe email to this cgi script (test.cgi). Once that is set, send a test email to the address set up for piping, if everything is set up correctly, a debugging message will be sent by the cgi script to:
<B><FONT COLOR=RED>$config_email</FONT></B> by way of <B><FONT COLOR=RED>$config_prog</FONT></B> (these are set directly by editting test.cgi)


</TD></TR></TABLE>
</TD></TR></TABLE>   

</TD></TR></TABLE>
</TD></TR></TABLE>   
    
~;

########## DB CHECK ##########
sub db_check {

unless ($INPUT->{'domain'} && $INPUT->{'usr'} && $INPUT->{'pwd'} && $INPUT->{'db'}) {
    print qq~
    <TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk" width="100%">
    <TR><TD><TABLE cellpadding=10 cellspacing=1 border=0 width="100%">
    <TR><TD CLASS="tblmain">
    <B>Please fill out all fields and submit again.
    <BR>
    $INPUT->{'domain'} && $INPUT->{'usr'} && $INPUT->{'pwd'} && $INPUT->{'db'}
    </TD></TR></TABLE>
    </TD></TR></TABLE> 

    </TD></TR></TABLE>
    </TD></TR></TABLE>
    ~;
    exit;
}

print qq~
<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk" width="100%">
<TR><TD><TABLE cellpadding=10 cellspacing=1 border=0 width="100%">
<TR><TD CLASS="tblmain">
<B>Trying to connect to database</B>
<BR><BR>
~;

eval { require DBI; };

if ($@) {
    print qq~
    <B><FONT COLOR=RED>Fatal error requiring perl modules</FONT></B> - $@
    </TD></TR></TABLE>
    </TD></TR></TABLE> 

    </TD></TR></TABLE>
    </TD></TR></TABLE>
    ~;
    exit;
}

my $driver = "mysql";
unless ($dbh = DBI->connect("DBI:$driver:$INPUT->{'db'}:$INPUT->{'domain'}", "$INPUT->{'usr'}", "$INPUT->{'pwd'}")) {
    print qq~
    <B><FONT COLOR=RED>Can not connect to sql server.</FONT></B> - $DBI::errstr
    </TD></TR></TABLE>
    </TD></TR></TABLE> 

    </TD></TR></TABLE>
    </TD></TR></TABLE>
    ~;
    exit;
}

$dbh->{'Warn'} = 0;
	
print qq~
<FONT COLOR=GREEN><B>Database connection susessful</B></FONT>
<BR><BR>
<B>Current MySQL threads</B>
<BR>
<TABLE class="tbldrk" border=0 cellpadding=0 cellspacing=0>
<TR><TD><TABLE cellpadding=2 cellspacing=1 border=0 width=100%>
<TR>
~;

my $sth = $dbh->prepare("SHOW PROCESSLIST");
$sth->execute;
 
foreach(@{ $sth->{NAME} }) {
    print "<td CLASS=\"tbltop\" align=center NOWRAP>$_</td>\n";
}

print "</TR>";

while( my $row = $sth->fetch ) {
    print "<tr class=\"tblmain\">";
    foreach( @$row ) {
        print "<td NOWRAP>", ($_ ? $_ : '&nbsp;'), "</td>\n";
    }
    print "</tr>";
}
$sth->finish;

print qq~
</TABLE></TD></TR></TABLE>
<BR><BR>
<B>MySQL appears to be active and operational.</B>
<BR>
</TD></TR></TABLE>
</TD></TR></TABLE> 

</TD></TR></TABLE>
</TD></TR></TABLE>
~;

exit;
}

sub pipe_email {

undef $/;
$buffer = <STDIN>;
$/="\n";

$email = $buffer;


open(MAIL, "|$config_prog -t");
print MAIL "To: $config_email \n";
print MAIL "From: $config_email \n";
print MAIL "Subject: Test Script piping results \n\n";
if ($email) {
	print MAIL "Looks like everything is working perfectly...\n\n";
	print MAIL "Here is a copy of the email you just sent.\n\n------------------------\n$email\n\n";
}
else {
	print MAIL "ERROR - The script was excuted at some point, but the text of the email was not available to the program.\n\n";
	print MAIL "Please double check your set up and try again...\n\n";
}
print MAIL "-----------------------\nENV Variables\n-----------------------\n\n";
while ((my $key, my $value) = each( %ENV) ) {
	print MAIL "$key -- $value \n";
}
print MAIL "\n\n";
close (MAIL);
exit;

}


########## PARSE INPUT #########
sub get_input {

my $GET_INPUT;
my $buffer = '';
my @pairs = '';
my $ftype = '';


if ($ENV{'CONTENT_LENGTH'}) {
	read(STDIN, my $buffer, $ENV{'CONTENT_LENGTH'});
	$GET_INPUT->{'buffer'} = $buffer;

	push(@pairs,split(/&/, $buffer));
	$ftype = "POST";
}

### FORM DATA ###
if ($ENV{'QUERY_STRING'}) { 
	push(@pairs,split(/&/,$ENV{'QUERY_STRING'}));
	$ftype = "GET" unless $ftype;
}

foreach (@pairs) {
	(my $name, my $value) = split(/=/, $_);
	$value =~ tr/+/ /;
	$value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
	$name =~ tr/+/ /;
	$name =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
	#$name =~ s/\.[x|y]$//;
	if ($GET_INPUT->{$name}) { $GET_INPUT->{$name} .= ",".$value; }
	else { $GET_INPUT->{$name} = $value; }
}



return($GET_INPUT);
}

sub header {

print "Content-type: text/html\n\n";

print qq~
<HTML>
<HEAD>
<TITLE>Test Script -- Solution Scripts</TITLE>
<style type="text/css">
//<!--

body {  font-family: Arial, Helvetica, sans-serif; font-size: 12px;}

BODY {
    SCROLLBAR-BASE-COLOR: #666699; 
    SCROLLBAR-ARROW-COLOR: #CCCCCC; 
}

A:link    {  text-decoration: none; color: blue}
A:visited {  text-decoration: none; color: blue}
A:hover   {  text-decoration: underline; color: red}

table.tblght { background-color: #CCCCCC; } 
table.tbldrk { background-color: #000000; } 
table.tblmid { background-color: #666666; }

td   {  font-family: Arial; font-size: 9pt;}
td.tblsml   {  font-family: Arial; font-size: 8pt;}
td.tblstat   { font-size: 3pt;}

tr.tbltop { color: #FFFFFF; background-color: #666699; }
tr.tbltop A:link { color: white}
tr.tbltop A:link { color: white}
tr.tbltop A:visited { color: white}

td.tbltop { color: #FFFFFF; background-color: #666699; }
td.tbltop A:link { color: white}
td.tbltop A:link { color: white}
td.tbltop A:visited { color: white}

tr.tblhdr { color: #000000; background-color: #E1E1E1; }
tr.tblmain { color: #000000; background-color: #FFFFFF; }

td.tblhdr { color: #000000; background-color: #E1E1E1; }
td.tblmain { color: #000000; background-color: #FFFFFF; }

.nav {  font-family: Verdana, Arial, Helvetica, sans-serif; color: #000000}
.std { font-family: Arial, Helvetica, sans-serif; font-size: 8pt;}
.hlght {  color: red; font-weight: bold; font-size: 14pt}
.lrg { font-family: Arial, Helvetica, sans-serif; font-size: 18px;}

//-->
</style>
</HEAD>

<BODY BRCOLOR=WHITE>

<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tblght" width="80%" ALIGN=center>
<TR><TD><TABLE cellpadding=8 cellspacing=1 border=0 width="100%">

<TR><TD CLASS="tblmain">

<TABLE cellpadding=0 cellspacing=0 border=0 CLASS="tbldrk" width="100%">
<TR><TD><TABLE cellpadding=5 cellspacing=1 border=0 width="100%">
<TR><TD CLASS="tbltop">
<FONT CLASS="lrg"><B>&gt;&gt; Solution Scripts :: Test Script</B></FONT>
</TD></TR></TABLE>
</TD></TR></TABLE>

</TD></TR>
<TR><TD CLASS="tblmain">
~;

}

1;