#!/usr/bin/perl
#
# This script is used to analyze the report file from the object manager.
#
# Usage:
#
#   perl object-manager.pl < /path/to/the/report
#
# Example of input file:
#
#   +0x56149a3717e0 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:27]
#   +0x56149a371ac0 M k [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:29]
#   +0x7ffe9961f260 P k [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:31]
#   +0x7ffe9961f268 P d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:34]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   +0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a371e50 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:42]
#   -0x56149a3717e0 M d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:27]
#   -0x56149a371ac0 M k [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:29]
#   -0x7ffe9961f260 P k [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:31]
#   -0x7ffe9961f268 P d [/home/dev/projects/CX_Lib/tests/src/test_CX_ObjectManager.c:34]




use strict;
use warnings FATAL => 'all';

my %mem = ();
my @warnings = ();
my @errors = ();
my @nils = ();

while (<STDIN>) {
    chomp;

    # ---------------------------------------------------------------
    # ADD detected
    # ---------------------------------------------------------------

    if ($_ =~ m/^\+(0x[0-9a-f]+) (P|M) (d|k) \[([^:]+):(\d+)\]$/) {
        my $pointer = $1;
        my $type = $2;
        my $action = $3;
        my $file = $4;
        my $line = $5;

        if (($type eq 'P') && ($action eq 'k')) {
            # Add the address of a point as a result.
            push(@warnings, "${file}:${line}: you identify the result of a function using the address of a pointer (${pointer}/${type}/${action})! It may be OK (if you use \"realloc()\"). But it is certainly unusual!");
        }

        $mem{$pointer} = {
            count => 0,
            type => $type,
            action => $action,
            file => $file,
            line => $line } unless exists($mem{$pointer});
        $mem{$pointer}->{count} += 1;
        next;
    }

    if ($_ =~ m/^#(0x[0-9a-f]+) (P|M) (d|k) \[([^:]+):(\d+)\]$/) {
        my $pointer = $1;
        my $type = $2;
        my $action = $3;
        my $file = $4;
        my $line = $5;

        if (($type eq 'P') && ($action eq 'k')) {
            # Add the address of a point as a result.
            push(@errors, "${file}:${line}: you've already declared the object under the responsibility of the manager (${pointer}/${type}/${action})!");
        }
        next;
    }

    if ($_ =~ m/^\+\(nil\) (P|M) (k|d) \[([^:]+):(\d+)\]$/) {
        my $type = $1;
        my $action = $2;
        my $file = $3;
        my $line = $4;

        if (($type eq 'P')) {
            push(@errors, "${file}:${line}: you declare a pointer which address is NULL under the manager responsibility (action=${action}). This should *NOT* happen!");
            next;
        }
        if (($type eq 'M')) {
            push(@warnings, "${file}:${line}: you declare a pointer which value is NULL under the manager responsibility (action=${action}). This is weird. You should look at it!");
            next;
        }

        next;
    }

    # ---------------------------------------------------------------
    # DSP detected
    # ---------------------------------------------------------------

    if ($_ =~ m/^\-(0x[0-9a-f]+) (P|M) (k|d) \[([^:]+):(\d+)\]$/) {
        my $pointer = $1;
        my $type = $2;
        my $action = $3;
        my $file = $4;
        my $line = $5;

        if (! exists($mem{$pointer})) {
            push(@errors, "${file}:${line}: unallocated pointer being freed (${pointer}/${type}/${action}). This should *NOT* happen!");
            next;
        }

        if (! exists($mem{$pointer})) {
            $mem{$pointer} = {
                count => 0,
                type => $type,
                action => $action,
                file => $file,
                line => $line }
        }
        $mem{$pointer}->{count} -= 1;
        next;
    }

    if ($_ =~ m/^\-\(nil\) (P|M) (k|d) \[([^:]+):(\d+)\]$/) {
        my $type = $1;
        my $action = $2;
        my $file = $3;
        my $line = $4;

        if (($type eq 'P')) {
            push(@errors, "${file}:${line}: the pointer to free has a NULL address (action=${action}). This should *NOT* happen!");
            next;
        }
        if (($type eq 'M')) {
            push(@warnings, "${file}:${line}: the pointer value to free is NULL (action=${action}). This is weird. You should look at it!");
            next;
        }

        next;
    }

    print("ERROR: unexpected line <$_>. You may have placed the object under the manager responsibility **before** it was allocated!\n");
    exit 1;
}

my $mem_leak_detected = 0;
foreach my $ptr (keys %mem) {
    if (0 != $mem{$ptr}->{count}) {
        $mem_leak_detected += 1;
        print(sprintf("${ptr} %d [%s:%d]\n",
            $mem{$ptr}->{count},
            $mem{$ptr}->{file},
            $mem{$ptr}->{line}));
    }
}

if (scalar(@nils) > 0) {
    printf("NULL pointers:\n");
    foreach my $rec (@nils) {
        print("* ${rec}\n");
    }
}

my $count = 0;
if (scalar(@warnings) > 0) {
    printf("Warnings:\n");
    foreach my $rec (@warnings) {
        $count += 1;
        print("[$count] ${rec}\n");
    }
    print("\n");
}

$count = 0;
if (scalar(@errors) > 0) {
    printf("Errors:\n");
    foreach my $rec (@errors) {
        $count += 1;
        print("[$count] ${rec}\n");
    }
    print("\n");
}

if (0 == scalar(@nils) && 0 == scalar(@warnings) && 0 == scalar(@errors) && !$mem_leak_detected) {
    print("TOTAL SUCCESS\n");
    exit 0;
}

if (!scalar(@errors) && !$mem_leak_detected) {
    print("\nPROBABLE SUCCESS (please pay attention to the warnings)\n");
    exit 0;
}

print("Potential memory leak detected!\n");
exit 1;
