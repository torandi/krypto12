##
# A program that reads a file containing plaintext numbers that in pairs of
# two makes up encrypted ascii characters.
#
# It can apply a range of single character keys 
# and finally perform one-to-one replacement of characters in the decoded output

#####
# SETTINGS
#####

#key_range = (0..0x7f) # Try all keys
key_range = [0x6e]     # Run with one key only

# One-to-one replacement map
replace_map = {
  "_"=>"o",
  "O"=>" ",
  "?" => "O",
  "#"=>"\n"
}

# Print the newlines of the original input
print_newline = false 

# Use the replacement map on the decoded characters
perform_replace = true

####
# END SETTINGS
####

f = File.open(ARGV[0])

data = f.read

pos = 0
hex_str = "00"


key_range.each do |key|
  puts "\n================ BEGIN KEY #{key} =================\n"
  data.each_char do |c|
    if c != "\n" 
      hex_str[pos]=c;
      pos+=1
      if(pos == 2)
        num = (hex_str.to_i(16)+key)%0x7f
        char = num.chr
        if(perform_replace && replace_map[char])
          char = replace_map[char]
        end
        print char
        pos = 0;
      end
    elsif print_newline
      puts "\n"
    end
  end
  puts "\n================ END KEY #{key} =================\n"
end
