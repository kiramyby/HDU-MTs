Status SubString(SString &Sub, SString S, int pos, int len) {
    if (pos + len - 1 > length(S) || pos < 1 || len < 0 || pos > S[0]) {
        return ERROR;
    }
    if (len == 0) {
        Sub[0] = 0;
        Sub[1] = '\0';
        return OK;
    }
    malloc(Sub, len + 1);
    for (int i = pos; i < pos + len; i++) {
        Sub[i - pos] = S[i];
    }
    Sub[len] = '\0';
    Sub[0] = len;
    return OK;
}