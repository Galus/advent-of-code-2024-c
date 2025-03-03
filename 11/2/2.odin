package main

import "core:fmt"
import "core:math"
import "core:os"
import "core:strconv"
import "core:strings"

get_digits :: proc(n: int) -> int {
	nf64 := f64(n)
	digit_count := int(math.floor(math.log10(nf64))) + 1
	return digit_count
}

split_integer :: proc(n: int) -> (int, int) {
	if n < 0 {
		return -1, -1
	}
	n_float := f64(n)
	digit_count := int(math.floor(math.log10(n_float))) + 1
	if digit_count % 2 == 0 {
		divisor := int(math.pow(10.0, f64(digit_count / 2)))
		first_part := n / divisor
		second_part := n % divisor
		return first_part, second_part
	}
	return -1, -1
}

parse_stones :: proc(input: string) -> (stones: map[int]int) {
	stone_strs := strings.split(input, " ", context.temp_allocator)
	for str in stone_strs {
		value := strconv.atoi(str)
		stone_count, ok := stones[value]
		if !ok {
			stones[value] = 1
		}
		stones[value] = stone_count + 1
	}
	return
}

simulate_blink :: proc(stones: map[int]int) -> (new_stones: map[int]int) {
	for stone_value, stone_count in stones {
		if stone_value == 0 {
			new_stones[1] += stone_count
			continue
		}
		digits := get_digits(stone_value)
		if (digits % 2 == 0) {
			half := digits / 2
			first := stone_value / int(math.pow(10, f64(half)))
			second := stone_value % int(math.pow(10, f64(half)))
			new_stones[first] += stone_count
			new_stones[second] += stone_count
			continue
		}
		new_stones[stone_value * 2024] += stone_count
	}
	delete(stones)

	return
}

main :: proc() {
	using fmt
	part1: int = 0
	stones := [dynamic]int{}
	blinks := 75

	if data, ok := os.read_entire_file("input.txt"); ok {
		data_str := string(data)
		data_str_trimmed := strings.trim(data_str, "\n")
		stones := parse_stones(data_str_trimmed)
		defer delete(stones)

		for _ in 0 ..< blinks {
			stones = simulate_blink(stones)
		}
		sum := 0
		for _, stone_count in stones {
			sum += stone_count
		}
		println("part1: ", sum)
	}

}
