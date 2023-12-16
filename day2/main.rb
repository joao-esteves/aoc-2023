# frozen_string_literal: true

# Determine which games would have been possible if the bag had been loaded
# with only 12 red cubes, 13 green cubes, and 14 blue cubes. What is the sum
# of the IDs of those games?

# Line format:
# Game <ID>: [<RED_n> red, <BLUE_n> blue, <GREEN_n> green][multiple times separated by ;]

File.readlines('input.txt', chomp: true).each do |line|
  game_str, combos = line.split ':'
  game_id = game_str.split(' ')[1]
  combos = combos.split ';'
end
