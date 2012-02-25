#####
# SETTINGS
#####

alphabet = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ_"

# True: newline after key.length chars, usefull for finding errors in key
# False: Use newline_char for newline, usefull for final output
newline_at_key_end=false

space_char = "_"
newline_char = "#"

####
# END SETTINGS
####

f = File.open(ARGV[0])
key = ARGV[1]

key_index = 0

data = f.read

data.each_char do |c|
  alphabet_index = alphabet.index c
  if alphabet_index
    g = alphabet.index key[key_index] # find key
    key_index = (key_index+1)%key.length # rotate key index
    d = (alphabet_index-g)%alphabet.length # apply key
    letter = alphabet[d]
    # Replace space and newline characters
    letter = " " if letter == space_char
    letter = "\n" if !newline_at_key_end && letter == newline_char
    # Output
    print letter

    if key_index == 0 && newline_at_key_end
      puts ""
    end
  end
end
