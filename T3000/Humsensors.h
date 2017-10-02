
#ifdef T3000_HUMSENSORS
namespace T3000_HumSensor {
#endif

 

/**
 */
class HumSensor : public Accessor {
	// Private so HumSensor objects can not be copied
	HumSensor(const HumSensor &source) : Accessor(), props(source.props) {}
	HumSensor &operator=(const HumSensor &) { return *this; }

protected:
	Document *pdoc;
	PropSet &props;
	WindowID id;
	int lenDoc;

	char styleBuf[bufferSize];
	int validLen;
	char chFlags;
	char chWhile;
	unsigned int startSeg;
	int startPosStyling;
	int mask;

	bool InternalIsLeadByte(char ch);
	void Fill(int position);

public:
	HumSensor(Document *pdoc_, PropSet &props_, WindowID id_=0) : 
		Accessor(), pdoc(pdoc_), props(props_), id(id_),
		lenDoc(-1), validLen(0), chFlags(0), chWhile(0), 
		startSeg(0), startPosStyling(0),
		mask(127) { // Initialize the mask to be big enough for any lexer.
	}
	~HumSensor();
	bool Match(int pos, const char *s);
	char StyleAt(int position);
	int GetLine(int position);
	int LineStart(int line);
	int LevelAt(int line);
	int Length();
	void Flush();
	int GetLineState(int line);
	int SetLineState(int line, int state);
	int GetPropertyInt(const char *key, int defaultValue=0) { 
		return props.GetInt(key, defaultValue); 
	}
	char *GetProperties() {
		return props.ToString();
	}
	WindowID GetWindow() { return id; }

	void StartAt(unsigned int start, char chMask=31);
	void SetFlags(char chFlags_, char chWhile_) {chFlags = chFlags_; chWhile = chWhile_; };
	unsigned int GetStartSegment() { return startSeg; }
	void StartSegment(unsigned int pos);
	void ColourTo(unsigned int pos, int chAttr);
	void SetLevel(int line, int level);
	int IndentAmount(int line, int *flags, PFNIsCommentLeader pfnIsCommentLeader = 0);
	void IndicatorFill(int start, int end, int indicator, int value);
};

#ifdef SCI_NAMESPACE
}
#endif
