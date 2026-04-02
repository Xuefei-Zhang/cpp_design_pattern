#include "state_patterns.h"

#include <cassert>
#include <iostream>
#include <string>

static void testInitialAndDraftEditFlow() {
    DocumentWorkflow doc("Product Roadmap");
    doc.initialize();

    assert(doc.getStateName() == "Draft");
    assert(doc.getVersion() == 0);
    assert(!doc.isDirty());

    doc.edit("Q1 goals. ");
    assert(doc.getStateName() == "Draft");
    assert(doc.isDirty());
    assert(doc.getContent().find("Q1 goals") != std::string::npos);

    doc.submitForReview();
    assert(doc.getStateName() == "Review");
}

static void testReviewTransitions() {
    DocumentWorkflow doc("API Specification");
    doc.initialize();
    doc.edit("v1 draft. ");
    doc.submitForReview();

    const std::string beforeBlockedEdit = doc.getContent();
    doc.edit("This should be blocked in review.");
    assert(doc.getContent() == beforeBlockedEdit);

    doc.reject();
    assert(doc.getStateName() == "Draft");

    doc.edit("Applied reviewer feedback. ");
    doc.submitForReview();
    doc.approve();

    assert(doc.getStateName() == "Published");
    assert(doc.getVersion() == 1);
    assert(!doc.isDirty());
}

static void testPublishedBehaviorAndRevisions() {
    DocumentWorkflow doc("Engineering Handbook");
    doc.initialize();
    doc.edit("Initial handbook content. ");
    doc.submitForReview();
    doc.approve();

    assert(doc.getStateName() == "Published");
    const int publishedVersion = doc.getVersion();

    doc.submitForReview();
    assert(doc.getStateName() == "Published");
    assert(doc.getVersion() == publishedVersion);

    doc.edit("Patch after release. ");
    assert(doc.getStateName() == "Draft");
    assert(doc.isDirty());

    doc.submitForReview();
    doc.approve();

    assert(doc.getStateName() == "Published");
    assert(doc.getVersion() == publishedVersion + 1);
}

static void testArchiveIsTerminal() {
    DocumentWorkflow doc("Legacy Guide");
    doc.initialize();
    doc.edit("Will be archived. ");
    doc.archive();

    assert(doc.getStateName() == "Archived");
    const std::string archivedContent = doc.getContent();
    const int archivedVersion = doc.getVersion();
    const bool archivedDirty = doc.isDirty();

    doc.edit("cannot edit");
    doc.submitForReview();
    doc.approve();
    doc.reject();
    doc.archive();

    assert(doc.getStateName() == "Archived");
    assert(doc.getContent() == archivedContent);
    assert(doc.getVersion() == archivedVersion);
    assert(doc.isDirty() == archivedDirty);
}

int main() {
    std::cout << "=== State Pattern Demo: Document Workflow ===\n\n";

    testInitialAndDraftEditFlow();
    testReviewTransitions();
    testPublishedBehaviorAndRevisions();
    testArchiveIsTerminal();

    std::cout << "All State pattern tests passed successfully!\n";
    std::cout << "Verified:\n";
    std::cout << "- State-specific behavior for Draft/Review/Published/Archived\n";
    std::cout << "- Context delegation to current state\n";
    std::cout << "- Valid and invalid transitions\n";
    std::cout << "- Revision lifecycle with publish/archive flow\n";

    return 0;
}
