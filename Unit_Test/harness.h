#ifndef UNIT_TEST_HARNESS_H
#define UNIT_TEST_HARNESS_H

void harness_reset(void);
void harness_expect(int condition, const char *file, int line, const char *expr);
int harness_failures(void);
int harness_passes(void);

#define EXPECT(cond) harness_expect((cond) ? 1 : 0, __FILE__, __LINE__, #cond)

#endif
