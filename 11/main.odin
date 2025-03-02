package main

import "core:container/intrusive/list"
import "core:fmt"
import "core:math"
import "core:os"
import "core:strconv"
import "core:strings"

states :: enum {
	REPLACE_ZERO_WITH_ONE,
	REPLACE_BY_2024X,
	REPLACE_WITH_SPLIT_TWO,
	STATES_LENGTH,
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

Stone :: struct {
	node:  list.Node,
	value: int,
}

main :: proc() {
	using fmt
	part1 := 0
	stones := [dynamic]int{}
	l: list.List
	stst := Stone {
		value = -1,
	}
	list.push_back(&l, &stst.node)

	if data, ok := os.read_entire_file("input.txt"); ok {
		data_str := string(data)
		data_str_trimmed := strings.trim(data_str, "\n")
		stones_stringarray := strings.split(data_str_trimmed, " ")
		stones_digitslengtharray := [dynamic]int{}

		for stone, index in stones_stringarray {
			stone_int := strconv.atoi(stone)
			append(&stones, stone_int)
			append(&stones_digitslengtharray, len(stone))
			s := Stone {
				value = stone_int,
			}
			list.push_back(&l, &s.node)
			println("Added stone ", stone, " to list")
		}

		//println("len:", len(stones_stringarray))
		//println("stones:", stones)
		println("stones_stringarray:", stones_stringarray)
		//println("stones_digitslengtharray:", stones_digitslengtharray)
		//println("stone 1 minus stone 2:", stones[0] - stones[1])
		a, b := split_integer(1337)

		println()

		foo := Stone {
			value = 55,
		}
		bar := Stone {
			value = 33,
		}
		list.push_back(&l, &foo.node)
		list.push_back(&l, &bar.node)

		iter := list.iterator_head(l, Stone, "node")
		stone_count := 0
		for sss in list.iterate_next(&iter) {
			fmt.println("List", stone_count, ":", sss.value)
			stone_count += 1
		}

		//one := Stone {
		//	value = 1,
		//}
		//two := Stone {
		//	value = 2,
		//}
		//r: list.List
		//println("list", r)
		//list.push_back(&r, &one.node)
		//println("Added stone ", one, " to list", r)
		//list.push_back(&r, &two.node)
		//println("Added stone ", two, " to list", r)
		//
		//iter2 := list.iterator_head(r, Stone, "node")
		//for s in list.iterate_next(&iter2) {
		//	fmt.println("rip", s.value)
		//}


		//for stone, index in stones {
		//	println("stone: ", stone)
		//	if stone == 0 {
		//		stones[index] = 1
		//	}
		//	println("length: ", stones_digitslengtharray[index])
		//	if stones_digitslengtharray[index] % 2 == 0 {
		//		first, second := split_integer(stone)
		//	}
		//	// TODO: Realizing this may be a linked list
		//
		//
		//	//switch stone % 2 {
		//	//case REPLACE_ZERO_WITH_ONE:
		//	//	break
		//	//case REPLACE_BY_2024X:
		//	//	break
		//	//case REPLACE_WITH_SPLIT_TWO:
		//	//	break
		//	//case STATES_LENGTH:
		//	//	break
		//	//case:
		//	//	// default
		//	//	println("Bad stone")
		//	//	return
		//	//}
		//}

	}


	fmt.println("Part1: ", part1)
}
