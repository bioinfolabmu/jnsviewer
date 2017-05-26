#!/usr/bin/perl -w
use strict;
my $usage = "This program get the sequence from the genome and gene annotation from the whole annotation gff file based on the position.\n"
	."perl get_seq_gff.pl <genome_sequence> <annotation_gff_file> <chrom> <start> <end> <strand> <seq_out> <gff_out>\n";
	
my $genome = shift or die $usage;
my $gff = shift or die $usage;
my $chrom = shift or die $usage;
my $start = shift or die $usage;
my $end = shift or die $usage;
my $strand = shift or die $usage;
my $seq_out = shift or die $usage;
my $gff_out =shift or die $usage;

my $line;

#test start and end
if($start =~ /\D/ || $end =~ /\D/ || $start =~ /^0/ || $end =~ /^0/) {
	die("start or end posisition not legal\n");
}

#get seq
my $flag = 0;
my $seq = "";
my $pos = 0;
my $pre = 0;
open IN,"<$genome" or die "Couldn't open $genome!\n";
while($line = <IN>) {
	if($line =~ /^>(\S+)/) {
		if($chrom eq $1) {
			$flag = 1;
			#print $chrom;
		} else {
			if($seq ne "") {
				#end out of range
				die("end position out of range.\n");				
			}			
			$flag = 0;
		}
	}
	else {		
		if($flag) {
			chomp $line;
			$pre = $pos + 1;
			$pos = $pos + length($line);
			if($pos >= $start) {
				if($start >= $pre) {
					#first line
					$line = substr($line, $start-$pre);
				}				
				$seq .= $line;
				if($pos >= $end) {
					#last line
					$seq = substr($seq, 0, $end-$start+1);				
					last;
				}
			}
		}
	}
}
close IN;

open OUT,">$seq_out" or die "Couldn't open $seq_out!\n";

if($seq eq "") {
	die("start position out of range.\n");
}

if($strand eq '-') {
	$seq = reverse($seq);
	$seq =~ tr/ATCG/TAGC/;
}

print OUT ">$chrom:$start:$end:$strand\n";
print OUT $seq."\n";
close OUT;

#get gff file
open IN,"<$gff" or die "Couldn't open $gff\n!";
open OUT,">$gff_out" or die "Couldn't open $gff_out!\n";

my $p = $end - $start + 2 ;
while($line = <IN>) {
	chomp $line;
	if($line =~ /^#/) {
		next;
	}
		
	#find chrom
	$line =~ /^(\S+)/;
	if($1 eq $chrom) {
		
		#only extract intron and exon
		if($line =~ /exon/) {
			my @lines = split(/\t/, $line);
			my $new_start;
			my $new_end;			
		
			#feature is within the range
			if($lines[3] >= $start && $lines[4] <= $end && $lines[6] eq $strand) {
				
				$new_start = $lines[3] - $start + 1;
				$new_end = $lines[4] - $start + 1;
			}
			#feature start is withing the range, end not
			elsif($lines[3] >= $start && $lines[3] <= $end && $lines[4] > $end && $lines[6] eq $strand) {
				$new_start = $lines[3] - $start + 1;
				$new_end = 1 + $end - $start;
				if($strand eq "-") {
					my $tmp = $p - $new_start;
					$new_start = $p - $new_end;
					$new_end = $tmp;
				}				
				print OUT "$chrom\t$lines[1]\t$lines[2]\t$new_start\t$new_end\t$lines[5]\t$strand\t$lines[7]\t$lines[8]\n";
				#end search
				last;
			} 
			#feature end is withing the range, start not
			elsif($lines[3] < $start && $lines[4] >= $start && $lines[4] <= $end && $lines[6] eq $strand) {
				$new_start = 1;
				$new_end = $lines[4] - $start + 1;
			}
			#feature include the range
			elsif($lines[3] < $start && $lines[4] > $end && $lines[6] eq $strand) {
				$new_start = 1;
				$new_end = 1 + $end - $start;
				if($strand eq "-") {
					my $tmp = $p - $new_start;
					$new_start = $p - $new_end;
					$new_end = $tmp;
				}				
				print OUT "$chrom\t$lines[1]\t$lines[2]\t$new_start\t$new_end\t$lines[5]\t$strand\t$lines[7]\t$lines[8]\n";
				#end search
				last;
			}
			#no feature in the range
			else {
				next;
			}
			
			if($strand eq "-") {
				my $tmp = $p - $new_start;
				$new_start = $p - $new_end;
				$new_end = $tmp;					
			}
			print OUT "$chrom\t$lines[1]\t$lines[2]\t$new_start\t$new_end\t$lines[5]\t$strand\t$lines[7]\t$lines[8]\n";
		}
	}	
}
	
close IN;
close OUT;