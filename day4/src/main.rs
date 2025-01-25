use std::fs::File;
use std::io::{BufRead, BufReader};
use regex::Regex;

fn card_from_str(string: String) {
    let re = Regex::new(r"Card +\d+: ((\d+\s+)+)| ((\d+\s+)+)").unwrap();
    let captures = re.captures(string);
    let winners = captures.get(2).unwrap().split_whitespace().map(|x| x.parse::<u32>().unwrap()).collect();
    let plays = captures.get(3).unwrap().split_whitespace().map(|x| x.parse::<u32>().unwrap()).collect();
    vec![winners, plays]
}

fn main() {
    let f = BufReader::new(File::open("input.txt").unwrap());
    // vector of cards
    // a card is a vector of winners and plays
    // winners and plays are vectors of numbers
    let arr: Vec<Vec<Vec<i32>>> = f.lines()
        .map(|line| card_from_str(line).unwrap())
        .collect();


    println!("Hello, world!");
}
