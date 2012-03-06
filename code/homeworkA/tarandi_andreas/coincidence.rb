##
# This program calculates the index of coincidence and related data
# Usage: ruby coincidence.rb input_file
# Author: Andreas Tarandi - taran at kth dot se

#####
# SETTINGS
#####

key_len_range = (1..18)
#key_len_range = [18]

alphabet = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ_"

# Set to true to calculate probability distrubution for each key combination and try to guess the key
calc_prob_distribution = false

# Threashold for coloring probability distrubution
green_th = 0.05
yellow_th = 0.04
####
# END SETTINGS
####

require "./english.rb" #File containing standard probability distrubution in english

require "colorize" # Gem colorize

f = File.open(ARGV[0])

data = f.read

key_len_range.each do |keylen|
  puts "Coincidence indices for key length #{keylen}"
  # Array of freq for each letter in the key
  freq_per_row = Array.new(keylen) {|i| Array.new(alphabet.length, 0) }
  pos = 0
  #Analyze the text:
  data.each_char do |c|
    alphabet_index = alphabet.index c
    if alphabet_index
      row = pos%keylen
      freq_per_row[row][alphabet_index]+=1;
      pos+=1
    end
  end

  if pos%keylen != 0
    puts "Warning, did not end on multiple of key length"
  end

  # Print table header
  printf("Row\t ioc \t");
  (0..alphabet.length-1).each do |a|
    printf("%2d:%c  ", a, alphabet[a])
  end 
  printf("\n")

  row_index = 1
  key = ""

  # For each row:
  freq_per_row.each do |freq|
    n = freq.reduce(0,:+) 
    # Calculate index of coincidence
    ioc = freq.reduce(0.0) do |memo, f| 
      # Calculate value for this char (and add memo)
      memo+(f.to_f*(f-1))/(n*(n-1))
    end
    # Print row number and index of coincidence
    printf("%3d\t%.3f\t", row_index, ioc)
    row_index+=1

    if calc_prob_distribution 
      best_val = 0
      best_let = ""
      green_let = ""
      # Try all shifts and calculate closeness to ideal distrubution
      (0..alphabet.length-1).each do |g|
        mg = 0.0
        (0..alphabet.length-1).each do |i| 
          p = $english_p[alphabet[i]] 
          gi = (i+g)%alphabet.length # apply shift
          mg += (p*freq[gi])/n
        end
        # Save the best value
        if mg > best_val
          best_val = mg
          best_let = alphabet[g]
        end

        # Output result
        output = "%.3f"
        if mg > green_th
          green_let+= alphabet[g]
          output = output.colorize(:green)
        elsif mg > yellow_th
          output = output.colorize(:yellow)
        end
        printf("#{output} ", mg);
      end
      # Print best guess
      key+=best_let
      printf("\t%c\t%s", best_let,green_let)
    end
    printf("\n")
  end
  puts ""
  puts "Proposed key: #{key}"
end
