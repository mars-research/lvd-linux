
run: blob-run blob
	./blob-run blob $(python blob-size.py blob.bin -o)

blob-run: blob-run.c
	gcc $^ -o $@

blob: blob.S
	as blob.S -o blob.bin &&
	objdump --output-target=binary \
	   --pad-to=$(python blob-size.py blob.bin -s) blob.bin blob