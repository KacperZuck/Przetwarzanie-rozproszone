public class Message {
    public static final int LOCK = 1;
    public static final int NOTIFY = 2;

    public int type;
    public String content;

    public Message(int type, String content) {
        this.type = type;
        this.content = content;
    }
}
