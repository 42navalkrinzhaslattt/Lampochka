import collections
import statistics

input_file = "/Users/lap/Documents/Arduino/lampochka/logs/09/background1_rain.txt"

gate_vals = {i: [] for i in range(14)}

with open(input_file, "r") as f_in:
    for line in f_in:
        if ",GATE," in line:
            parts = line.split("mv:")
            if len(parts) > 1:
                mv_st_part = parts[1]
                mv_part = mv_st_part.split(",st:")[0]
                mv_vals = mv_part.split(',')
                if len(mv_vals) >= 14:
                    try:
                        for g in range(14):
                            gate_vals[g].append(int(mv_vals[g]))
                    except ValueError:
                        pass

for g in range(14):
    if gate_vals[g]:
        counts = collections.Counter(gate_vals[g])
        
        most_common = counts.most_common(5)
        top_5_values = [val for val, count in most_common]
        median_top_5 = statistics.median(top_5_values)
        
        print(f"Gate {g}: Top 5 most frequent values: {most_common}, Median of these 5 values: {median_top_5}")
        
        print(f"Gate {g} full counts:")
        for val, count in sorted(counts.items()):
            print(f"  Value: {val} -> Count: {count}")
        print("-" * 20)
