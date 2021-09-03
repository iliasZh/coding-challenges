# Reassemble out of order data

#### This is a little coding problem
Here I\'ve used the `<filesystem>` for the first time and had fun :)

Problem statement:
Packets of a protocol are divided into several data segments during transmission.
The received data segments may be duplicated, lost, or out of order.
The format of each data segment is as follows:
* `[dataType|index|endFlag|contentLength|content]`
* `dataType`: data type of the segment, value range [0, 9]
* `index`: index of the segment in the packet, value range [0, 100\'000]
* `endFlag`: 1 if it\'s the last segment, 0 if not
* `contentLength`: value range [2, 5]

Given a batch of received data segments and a data type, reassemble the data packets of this data type.
The reassembly and output rules are as follows:
* `index` starts at 0 and is consecutive, and only the data segment with the largest index has an `endFlag` of 1
* If the reassembly is successful, the `content` of the data type is combined into a character string based on the `index` in ascending order and output. If the segment indexes are duplicate, ignore the duplicate one.
* If the reassembly failed, output the string `NA`

Input format:
An integer N on the first line denoting the number of data segments. The value range is [1, 1\'000\'000].
Each of the next N lines is a data segment: `[int int int int string]`.
The last line is an integer denoting the data type that needs to be reassembled.