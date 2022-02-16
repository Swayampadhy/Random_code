package main
import (
	"fmt"
	"math"
)

func main(){
	var number float64
	fmt.Print("Enter a float type number: ")
	fmt.Scan(&number)
	no:= math.Trunc(number)
	fmt.Print("The truncated no is:",no)
}