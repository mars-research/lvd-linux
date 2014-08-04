#! /bin/sh

# Index preserves order of values for same key

modindex -o tests/tmp/index << EOF
a 1
a 2
a 3
EOF

[ "`modindex -d tests/tmp/index`" = "a 1
a 2
a 3" ]

modindex -o tests/tmp/index << EOF
a 3
a 2
a 1
EOF

[ "`modindex -d tests/tmp/index`" = "a 3
a 2
a 1" ]

# Index preserves order of values across multiple keys

modindex -o tests/tmp/index << EOF
*a 1
b* 2
* 3
EOF

[ "`modindex -w ba tests/tmp/index`" = "Found value(s):
1
2
3" ]

modindex -o tests/tmp/index << EOF
* 1
b* 2
*a 3
EOF

[ "`modindex -w ba tests/tmp/index`" = "Found value(s):
1
2
3" ]
