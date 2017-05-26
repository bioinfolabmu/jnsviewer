#!/usr/bin/perl -w
use strict;
my $usage = "This program convert the ct file generated by RNAstructure to normal ct file\nperl $ARGV[0] <input_file>";
my $file = shift or die $usage;

open IN,"<$file" or die "Couldn't open $file!";

my $line = <IN>;
chomp $line;

my @lines = split(/\s/, $line);

print $lines[0].' '.$lines[$#lines]."\n";

while($line = <IN>) {
	if($line =~ /ENERGY/) {
		last;
	} else {
		print $line;
	}
}

close IN;