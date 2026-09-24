fn main() {
    let s1 = String::from("hello");
    let s2 = s1; // shallow copy, s1 is moved to s2

    // Can't reference s1, it's been moved to s2
    // println!("{s1}, world!");
    println!("{s2}, world!");
}