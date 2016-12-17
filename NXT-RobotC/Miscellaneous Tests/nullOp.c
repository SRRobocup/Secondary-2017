task main()
{
	clearDebugStream();
	int i;
	long total = 0;
	long time;
	long iterations = 100000;
	for (int j = 0; j < iterations; j++) {
		time = nPgmTime;
		for(i = 0; nPgmTime - time < 1; i++ )
			noOp();
		writeDebugStreamLine("%d", i);
		total += i;
	}
	writeDebugStreamLine("Final: %d", total/iterations);
}
