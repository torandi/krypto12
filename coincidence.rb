#####
# SETTINGS
#####

key_len_range = (1..18)
#key_len_range = [3]

alphabet = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ_"

####
# END SETTINGS
####

f = File.open(ARGV[0])

data = f.read

key_len_range.each do |keylen|
  puts "Coincidence indices for key length #{keylen}"
  # Array of freq for each letter in the key
  freq_per_row = Array.new(keylen) {|i| Array.new(alphabet.length, 0) }
  pos = 0
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

  # Calculate index of coincidence for each row
  row_index = 1
  freq_per_row.each do |freq|
    n = freq.reduce(0,:+) 
    ioc = freq.reduce(0.0) do |memo, f| 
      # Calculate value for this char (and add memo)
      memo+(f.to_f*(f-1))/(n*(n-1))
    end
    puts "Row #{row_index}: #{ioc}"
    row_index+=1
  end
  puts ""
end
