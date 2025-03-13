


//vector::vector(vector&& rhs) {
//    /* Steal resources from rhs. */
//    elems = rhs.elems;
//    size = rhs.size;
//    capacity = rhs.capacity;
//
//    /* Destructively modify rhs to avoid having two objects sharing
//     * an underlying array.
//     */
//    rhs.elems = nullptr; // Note use of nullptr instead of NULL
//    rhs.size = 0;
//    rhs.capacity = 0;
//}