#!/usr/bin/perl
# Ralf's Chat - a Perl webchat script
# Copyright 1998-2002 Ralf Gueldemeister <ralf@ralfchat.com>
#
# This file is part of Ralf's Chat.
#
# Ralf's Chat is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Ralf's Chat is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Ralf's Chat; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Config Part
#
#######################

# forward script error messages to the browser, comment if the module is not available
use CGI::Carp qw(fatalsToBrowser);

# Masterpassword for special commands
$masterpassword = 'ankara'; 
# Your email addresse
$admin_email_addresse = 'cem.karaca@emu.edu.tr'; 
# Path to the mail-transport-agent
$mailprogramme = '/usr/lib/sendmail'; 
# set to 1 if you want to be mailed when a new user registers
$mail_on_new_registration = 0; 
# The type of log: 0 - no log, 1 - registrations, 2 - +logins, 3 - +messages, 4-6 - +debug
$logtype = 0;
# the maximal number of users in the chat at the same time, set to -1 if unlimited
$max_users = -1; 
# Number of days to keep memos
$days_to_keep_memos = 60; 
# set to 1 if you want flocking (file lock), this is not supported by Windows NT (set to 0)
$flocking = 1; 
# 0 - nick may only contain letters and numbers, 1 - nick may contain any character except ";/ :\?*<>|'
$international_nicks = 1; 
# language
$language = "english";
# number of days after old users will be deleted - set to -1 to never delete any users automatically
$days_del_old_users = 60;


# Directory Names

# data directory like "data"
$data_dir = "../../Libs/chat"; 
# html directory
$html_dir = "html"; 
# language directory
$lang_dir = "lang"; 
# directory for log files
$log_dir = "logs"; 

# Look And Feel

# chat refresh delay
$updatefrequency = 10;
# the minimum update frequency
$min_update_freq = 5;
# set to 1 if you want that old msgs are deleted when the chat is empty
$clear_old_msgs = 1; 
# number of maximal messages in the chat window
$message_limit = 20; 
# set to 1 if you want new messages printed on top, to work around the msie bug (default value for new users)
$new_msg_on_top = 0; 
# set to 1 if you want the select field for private msgs to be reset to "to all users" after msg was sent
$reset_private_msg_select = 0; 
# set to 1 if you want to see the currently chatting users on the login page
$show_users_on_login = 1; 
# maximum length of nickname and password
$max_input_length = 30; 
# set to 1 if you want posting time
$posting_time = 0;
# customizable posting time format - see 'man date' for details of the format
$posting_time_format = "%Y-%m-%d %T";
# time difference between server and local time in hours (may be negative)
$time_difference = 0;

# Script Names

# filename of the script like "chat.cgi"
$script_name   = "chat.cgi";   
# filename of the second script like "chat2.cgi"
$script2_name  = "chat2.cgi";  
# filename of the post message script
$scriptpm_name = "chatpm.cgi"; 
# filename of the still alive script
$scriptsa_name = "chatsa.cgi"; 
# filename of the show users script
$scriptsu_name = "chatsu.cgi"; 
# filename of the goto url script
$scriptgu_name = "chatgu.cgi"; 
#<DBI>
# filename of the stream script
$scriptst_name = "stream.cgi";
#</DBI>

# Files/Tables - don't need to be changed.

$data{'messages'} = "messages";
$data{'stillalive'} = "stillalive";
$data{'nicks'} = "nicks";
$data{'userinfo'} = "userinfo";
$data{'banned'} = "banned";
$data{'memo'} = "memo";

# <DBI>
# DBI (Data Base Interface)
# set to 1 if you want to use the DBI
$use_dbi = 0;
# user for DBI
$dbi_user = 'control'; 
# password for DBI
$dbi_password = 'ankara'; 
# database
$dbi_database = 'ralfchat'; 
# host
$dbi_host = 'localhost';
# DSN (Data Source Name for the DBI)
$dbi_dsn = "DBI:mysql:database=$dbi_database;host=$dbi_host"; 
# set to one if your DBMS supports primary keys (eg. MySQL)
$use_primary_keys = 1; 
# </DBI>

# HTML-Stuff

# The bodytag for html files
$html_bodytag = '<BODY BGColor=#000000 TEXT=#6699FF Link=#0000FF VLINK=#0000CC ALINK=#0000FF>'; 
# The style sheet definition used for all html files
$html_css = << "[END]"; 
<STYLE TYPE="text/css">
</STYLE>
[END]

# The title of your chat
$html_title = 'Ralf\'s Chat'; 
# The banner picture
$banner_picture = 'http://www.ralfchat.com/banner.jpg'; 
# The banner link
$banner_link = 'http://www.ralfchat.com'; 
# The banner url, you may set it to your banner rotation script, set to "" if you don't want any banner
$banner_url = "$script2_name?action=banner"; 
# the url for the logout-link
$logout_url = "$script_name"; 
# the size of the msg input field
$input_field_size = 30; 

# Actions
# just uncomment (remove leading '#') the lines in the brackets, and add as many actions as you like

%actions = (
#    'action' => '/image_path/action_image.gif',
#    'action2' => '/image_path/action2_image.gif',
);

# Smileys / Replace
# replaces text in messages, eg. smileys
# just uncomment the lines in the brackets, and add as many smileys as you like
%replace = (
#	    ';-)' => '<IMG SRC="/image_path/icon1.gif">',
#	    'replace this' => 'with this',
);

# Bad Word Filter

# Bad words are replaced with this one
$filter = '[censored]'; 
@bad_words = ( # uncomment the following line and change it to your needs
#	    'bad word 1', 'bad word 2',
);

# Userlevel
%ul = (
    'msg'		=> 0,
    'help'		=> 0,
    'about'		=> 0,
    'me'		=> 0,
    'whois'		=> 0,
    'memo'		=> 0,
    'away'		=> 0,
    'ip'		=> 1,
    'kick'		=> 1,
    'ban'		=> 1,
    'unban'		=> 1,
    'users'		=> 2,
    'deluser'	=> 2,
    'admin_memo'	=> 2,
    'userlevel'	=> 0,
);
%ul_names = (0 => 'normal', 1 => 'su', 2 => 'admin');
%short_ul_names = (0 => '', 1 => '[su]', 2 => '[adm]');

# defaul room, set to 0 if you don't want multiple rooms
$room = 0;

# rooms (room number => 'room name', room number > 0) 
%rooms = (
    1 => 'room 1',
    2 => 'room 2',
);

# welcome messages (room number => 'welcome message')
%welcome_msgs = (
    1 => 'You are in room 1!',
    2 => 'You are in room 2!',
);


# debug
$sa_timeout = 240; # min $sa_refresh
$sa_refresh = 60; # min $min_update_freq
$sid_length = 30; # max 50
%backup_fil = ('stillalive' => 1);


1;

# EOF
