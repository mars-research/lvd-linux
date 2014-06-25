
run: blob-run blob
	sudo ./blob-run blob `python blob-size.py blob.bin -o`

blob-run: blob-run.c
	gcc $^ -o $@

blob: blob.bin
	objcopy --output-target=binary \
	   --pad-to=`python blob-size.py blob.bin -s` blob.bin blob

blob.bin: blob.S
	as blob.S -o blob.bin

clean:
	rm -f blob blob.bin blob-run *~