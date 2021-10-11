#!/usr/bin/perl

use warnings;
use strict;

my $CWD = `pwd`;
my $DISPLAY = ":0";
chomp $CWD;

sub pod_run{
	my ($cmd) = @_;
	
	my $command = "podman run -v $CWD:/prog -v /tmp/.X11-unix:/tmp/.X11-unix -v /dev/dri:/dev/dri -e DISPLAY=$DISPLAY -it --rm gtk4 bash -c '$cmd'";
	
	return `$command`;
}

if($ARGV[0] eq "conf"){
	print pod_run("cd /prog && mkdir -p build && cd build && cmake ..");
}elsif($ARGV[0] eq "build"){
	print pod_run("cd /prog/build && make -j4");
}elsif($ARGV[0] eq "run"){
	print pod_run("cd /prog/build && ./Youtube");
}
