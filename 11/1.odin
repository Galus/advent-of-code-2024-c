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

processStone :: proc(l: ^list.List, s: ^Stone) {
	digits := get_digits(s.value)
	if s.value == 0 {
		fmt.println("\tsetting stone to 1")
		s.value = 1
	} else if digits % 2 == 0 {
		first, second := split_integer(s.value)
		fmt.println("\tsplitting", s.value, "to", first, "&", second)

		// create this node structure
		first_node := new(Stone)
		first_node.value = first
		second_node := new(Stone)
		second_node.value = second

		// 1 -> 2
		first_node.node.next = &second_node.node
		// 1 <- 2
		second_node.node.prev = &first_node.node
		// 1 <-> 2

		//  n-1 -> 1 
		if s.node.prev != nil {
			s.node.prev.next = &first_node.node
		} else {
			l.head = &first_node.node
			fmt.println("\tfirst node")
		}
		// n-1 -> 1 <-> 2

		//  2 <- n+1 
		if s.node.next != nil {
			s.node.next.prev = &second_node.node
		} else {
			l.tail = &second_node.node
			fmt.println("\tlast node")
		}
		// n-1 -> 1 <-> 2 <- n+1

		// n-1 <- 1
		first_node.node.prev = s.node.prev
		// n-1 <-> 1 <-> 2 <- n+1

		// 2 -> n+1
		second_node.node.next = s.node.next
		// n-1 <-> 1 <-> 2 <-> n+1

		//free(s)
	} else {
		fmt.println("\tmultiplying stone", s.value, "by 2024")
		s.value = s.value * 2024
	}
}

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

Stone :: struct {
	node:  list.Node,
	value: int,
}

printListValues :: proc(l: list.List, $T: typeid) {
	iter := list.iterator_head(l, T, "node")
	stone_count := 0
	for s in list.iterate_next(&iter) {
		fmt.print(s.value)
		if (l.tail != &s.node) {
			fmt.print(",")
		} else {
			fmt.println()
		}
	}

}

main :: proc() {
	using fmt
	part1 := 0
	stones := [dynamic]int{}
	l: list.List

	if data, ok := os.read_entire_file("input.txt"); ok {
		data_str := string(data)
		data_str_trimmed := strings.trim(data_str, "\n")
		stones_stringarray := strings.split(data_str_trimmed, " ")
		stones_digitslengtharray := [dynamic]int{}

		for stone, index in stones_stringarray {
			stone_int := strconv.atoi(stone)
			append(&stones, stone_int)
			append(&stones_digitslengtharray, len(stone))
			s := new(Stone)
			s.value = stone_int
			list.push_back(&l, &s.node)
		}

		println("blinks 0:", stones_stringarray)

		println("Initiating Blinks Process...")

		iter := list.iterator_head(l, Stone, "node")
		blinks := 1
		for blinks <= 25 {
			println("Blink", blinks)
			s, _ := list.iterate_next(&iter)
			for s != nil {
				println("processing", s.value)
				processStone(&l, s)
				s, _ = list.iterate_next(&iter)
			}

			print("List: ")
			printListValues(l, Stone)
			println()
			blinks = blinks + 1
			iter = list.iterator_head(l, Stone, "node")
		}


	}

	stone_count := 0
	iter := list.iterator_head(l, Stone, "node")
	s, _ := list.iterate_next(&iter)
	for s != nil {
		stone_count += 1
		s, _ = list.iterate_next(&iter)
	}
	print("\nFinal list: ")
	printListValues(l, Stone)
	println()
	println("part1:", stone_count) //55312 too low (oops used sample input)
}
