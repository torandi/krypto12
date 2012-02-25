#####
# SETTINGS
#####

alphabet = "#ABCDEFGHIJKLMNOPQRSTUVWXYZ_"

####
# END SETTINGS
####

f = File.open(ARGV[0])
key = ARGV[1]

key_index = 0

data = f.read

replace_map = {
  "_" => " "
}

data.each_char do |c|
  alphabet_index = alphabet.index c
  if alphabet_index
    g = alphabet.index key[key_index]
    key_index = (key_index+1)%key.length
    d = (alphabet_index-g)%alphabet.length
    letter = alphabet[d]
    letter = replace_map[letter] if replace_map[letter]
    print letter
  else
    print c
  end
end
