#ifndef STATE_PATTERNS_H
#define STATE_PATTERNS_H

#include <iostream>
#include <memory>
#include <string>

class DocumentWorkflow;

class State {
public:
    virtual ~State() = default;

    virtual void edit(DocumentWorkflow& context, const std::string& text) = 0;
    virtual void submitForReview(DocumentWorkflow& context) = 0;
    virtual void approve(DocumentWorkflow& context) = 0;
    virtual void reject(DocumentWorkflow& context) = 0;
    virtual void archive(DocumentWorkflow& context) = 0;
    virtual std::string getName() const = 0;
};

class DraftState;
class ReviewState;
class PublishedState;
class ArchivedState;

class DocumentWorkflow {
private:
    std::unique_ptr<State> currentState_;
    std::string title_;
    std::string content_;
    bool dirty_;
    int version_;

public:
    explicit DocumentWorkflow(const std::string& title)
        : currentState_(nullptr), title_(title), content_(""), dirty_(false), version_(0) {}

    void initialize();

    void transitionTo(std::unique_ptr<State> newState) {
        const std::string oldState = currentState_ ? currentState_->getName() : "None";
        const std::string newStateName = newState ? newState->getName() : "None";
        std::cout << "[CONTEXT] Transition: " << oldState << " -> " << newStateName << std::endl;
        currentState_ = std::move(newState);
    }

    void edit(const std::string& text) {
        if (currentState_) {
            currentState_->edit(*this, text);
        }
    }

    void submitForReview() {
        if (currentState_) {
            currentState_->submitForReview(*this);
        }
    }

    void approve() {
        if (currentState_) {
            currentState_->approve(*this);
        }
    }

    void reject() {
        if (currentState_) {
            currentState_->reject(*this);
        }
    }

    void archive() {
        if (currentState_) {
            currentState_->archive(*this);
        }
    }

    void appendContent(const std::string& text) {
        content_ += text;
        dirty_ = true;
    }

    void markPublished() {
        ++version_;
        dirty_ = false;
    }

    const std::string& getTitle() const { return title_; }
    const std::string& getContent() const { return content_; }
    bool isDirty() const { return dirty_; }
    int getVersion() const { return version_; }
    std::string getStateName() const { return currentState_ ? currentState_->getName() : "None"; }
};

class DraftState : public State {
public:
    void edit(DocumentWorkflow& context, const std::string& text) override;
    void submitForReview(DocumentWorkflow& context) override;
    void approve(DocumentWorkflow& context) override;
    void reject(DocumentWorkflow& context) override;
    void archive(DocumentWorkflow& context) override;
    std::string getName() const override { return "Draft"; }
};

class ReviewState : public State {
public:
    void edit(DocumentWorkflow& context, const std::string& text) override;
    void submitForReview(DocumentWorkflow& context) override;
    void approve(DocumentWorkflow& context) override;
    void reject(DocumentWorkflow& context) override;
    void archive(DocumentWorkflow& context) override;
    std::string getName() const override { return "Review"; }
};

class PublishedState : public State {
public:
    void edit(DocumentWorkflow& context, const std::string& text) override;
    void submitForReview(DocumentWorkflow& context) override;
    void approve(DocumentWorkflow& context) override;
    void reject(DocumentWorkflow& context) override;
    void archive(DocumentWorkflow& context) override;
    std::string getName() const override { return "Published"; }
};

class ArchivedState : public State {
public:
    void edit(DocumentWorkflow& context, const std::string& text) override;
    void submitForReview(DocumentWorkflow& context) override;
    void approve(DocumentWorkflow& context) override;
    void reject(DocumentWorkflow& context) override;
    void archive(DocumentWorkflow& context) override;
    std::string getName() const override { return "Archived"; }
};

inline void DocumentWorkflow::initialize() {
    transitionTo(std::make_unique<DraftState>());
}

inline void DraftState::edit(DocumentWorkflow& context, const std::string& text) {
    std::cout << "[DRAFT] Editing document '" << context.getTitle() << "'" << std::endl;
    context.appendContent(text);
}

inline void DraftState::submitForReview(DocumentWorkflow& context) {
    std::cout << "[DRAFT] Submit for review" << std::endl;
    context.transitionTo(std::make_unique<ReviewState>());
}

inline void DraftState::approve(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[DRAFT] Cannot approve directly. Submit for review first." << std::endl;
}

inline void DraftState::reject(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[DRAFT] Cannot reject. Document is not under review." << std::endl;
}

inline void DraftState::archive(DocumentWorkflow& context) {
    std::cout << "[DRAFT] Archive draft" << std::endl;
    context.transitionTo(std::make_unique<ArchivedState>());
}

inline void ReviewState::edit(DocumentWorkflow& context, const std::string& text) {
    (void)context;
    (void)text;
    std::cout << "[REVIEW] Direct editing is blocked during review. Reject to return to draft." << std::endl;
}

inline void ReviewState::submitForReview(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[REVIEW] Already in review." << std::endl;
}

inline void ReviewState::approve(DocumentWorkflow& context) {
    std::cout << "[REVIEW] Approved. Publishing document." << std::endl;
    context.markPublished();
    context.transitionTo(std::make_unique<PublishedState>());
}

inline void ReviewState::reject(DocumentWorkflow& context) {
    std::cout << "[REVIEW] Rejected. Returning to draft for updates." << std::endl;
    context.transitionTo(std::make_unique<DraftState>());
}

inline void ReviewState::archive(DocumentWorkflow& context) {
    std::cout << "[REVIEW] Archive document under review" << std::endl;
    context.transitionTo(std::make_unique<ArchivedState>());
}

inline void PublishedState::edit(DocumentWorkflow& context, const std::string& text) {
    std::cout << "[PUBLISHED] Creating a new draft revision from published version." << std::endl;
    context.transitionTo(std::make_unique<DraftState>());
    context.appendContent(text);
}

inline void PublishedState::submitForReview(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[PUBLISHED] Already published. Edit to create a new draft first." << std::endl;
}

inline void PublishedState::approve(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[PUBLISHED] Already approved and published." << std::endl;
}

inline void PublishedState::reject(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[PUBLISHED] Cannot reject published document. Edit and re-review instead." << std::endl;
}

inline void PublishedState::archive(DocumentWorkflow& context) {
    std::cout << "[PUBLISHED] Archiving published document." << std::endl;
    context.transitionTo(std::make_unique<ArchivedState>());
}

inline void ArchivedState::edit(DocumentWorkflow& context, const std::string& text) {
    (void)context;
    (void)text;
    std::cout << "[ARCHIVED] Cannot edit archived document." << std::endl;
}

inline void ArchivedState::submitForReview(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[ARCHIVED] Cannot submit archived document." << std::endl;
}

inline void ArchivedState::approve(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[ARCHIVED] Archived document cannot be approved." << std::endl;
}

inline void ArchivedState::reject(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[ARCHIVED] Archived document cannot be rejected." << std::endl;
}

inline void ArchivedState::archive(DocumentWorkflow& context) {
    (void)context;
    std::cout << "[ARCHIVED] Document is already archived." << std::endl;
}

/*
Example usage:

DocumentWorkflow doc("Architecture Decision");
doc.initialize();

doc.edit("Initial draft content\n");
doc.submitForReview();
doc.approve();

std::cout << doc.getStateName() << std::endl; // Published
*/

#endif
