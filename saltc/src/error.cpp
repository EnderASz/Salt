/**
 * error.h implementation
 *
 * @author EnderASz
 */
#include "../include/error.h"

#include "../include/utils.h"
#include <string>

using std::string;
using std::to_string;

namespace salt
{

BaseError::~BaseError() {}

InSourceError::InSourceError(
    const InStringPosition position,
    const SourceFile& source_file)
        :position(position),
        source_file(const_cast<SourceFile*>(&source_file)) {}

void InSourceError::setPosition(InStringPosition position) {
    this->position = position;
}

void InSourceError::setSource(SourceFile& source_file) {
    this->source_file = &source_file;
}

string InSourceError::getMessage() {
    return (
        "An error occured in '" +
        getSource()->getFilePath() +
        "' at line " +
        to_string(getPosition().line_idx + 1) +
        " at position " +
        to_string(getPosition().inline_idx + 1) +
        "."        
    );
}

InStringPosition InSourceError::getPosition() {return position;}

SourceFile* InSourceError::getSource() {return source_file;}


UnclosedCommentError::UnclosedCommentError(
    const InStringPosition position,
    const SourceFile& source_file)
        :InSourceError(position, source_file) {}

string UnclosedCommentError::getMessage() {
    return (
        "Unclosed comment in '" +
        getSource()->getFilePath() +
        "' at line " +
        to_string(getPosition().line_idx + 1) +
        " at position " +
        to_string(getPosition().inline_idx + 1) +
        "."        
    );
}

};