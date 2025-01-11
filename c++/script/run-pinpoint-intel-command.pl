#!/usr/bin/env perl

use strict;
use warnings;

use v5.14;

use lib qw(lib ../lib ../../lib);
use Utils qw(process_pinpoint_output);

my $preffix = shift || die "I need a prefix for the data files";
my $command = shift || die "I need a (single) command to run";
my $work = shift || die "I need type of work, (i)nitialization (g)enetic ops or (h)iff";
my $data_dir = shift || "data";
my $ITERATIONS = 30;
my ($mon,$day,$hh,$mm,$ss) = localtime() =~ /(\w+)\s+(\d+)\s+(\d+)\:(\d+)\:(\d+)/;
my $suffix = "$day-$mon-$hh-$mm-$ss";

open my $fh, ">", "$data_dir/$preffix-$suffix.csv";
say $fh "Platform,size,RAM,Core,Psys,seconds";

for my $l ( qw(512 1024 2048) ) {
  my $total_seconds;
  my $successful = 0;
  my @results;
  do {
    my $command = "$command -s $l -w $work";
    say $command;
    my $output = `pinpoint -- $command 2>&1`;
    say $output;
    my ( $ram, $cores, $psys, $gpu, $pkg, $seconds ) = process_pinpoint_intel_output $output;
    if ($gpu != 0 ) {
      $successful++;
      $total_seconds += $seconds;
      say "$preffix, $l, $ram, $cores, $psys";
      push @results, [$ram, $cores,$psys,$seconds];
    }
  } while ( $successful < $ITERATIONS );

  foreach  my $row (@results) {
    say join(", ", @$row);
    say $fh "$preffix, $l, ", join(", ", @$row);
  }
}
close $fh;
