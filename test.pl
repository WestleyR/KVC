#!/usr/bin/perl

use Time::HiRes;

my %hash;
my $index = $ARGV[0];

for (my $i=0; $i <= 1000000; $i ++) {
    $hash{"key_$i"} = "val_$i"
}



my $start = Time::HiRes::gettimeofday();
my $val = $hash{"key_$index"};
my $end = Time::HiRes::gettimeofday();
my $spent = $end - $start;

printf("key_$index=$val\n");
printf("time: %10.8f\n", $spent);
