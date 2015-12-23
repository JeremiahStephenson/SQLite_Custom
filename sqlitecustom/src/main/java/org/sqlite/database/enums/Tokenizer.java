package org.sqlite.database.enums;

/**
 * Created by jeremiahstephenson on 3/12/15.
 */
public enum Tokenizer {

    HTML_TOKENIZER("HTMLTokenizer"), CHARACTER_TOKENIZER("character");

    private String name;

    Tokenizer(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }
}