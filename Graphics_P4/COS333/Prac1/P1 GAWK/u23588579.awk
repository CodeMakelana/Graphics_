BEGIN {
    FS = ","
    print "=========================="
    print "Rectangular matrix totals"
    print "=========================="
    print ""
    print "Input file: input.txt"
}

{
    curr_row_total = 0

    for (i = 1; i <= NF; i++) {
        curr_row_total += $i
        column_totals[i] += $i
        grand_total += $i
    } 

    if (NF > max_columns) {
    max_columns = NF
    }

    print "Total for row " NR ": " curr_row_total
    
}

END {
    print ""
    for (i = 1; i <= max_columns; i++) {
        print "Total for column " i ": " column_totals[i] 
    }

    print ""
    print "Total for entire matrix: " grand_total 
}
