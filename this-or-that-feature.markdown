# This or That Idea Ranking Page Feature Proposal

## Overview
The "This or That" Idea Ranking Page is a new feature for OpenideaL, inspired by early social media engagement mechanisms (e.g., Facebook’s minimalist comparison interfaces). It presents users with two ideas from the OpenideaL ideation community, allowing them to vote for their preferred idea. The feature aims to boost user engagement, enhance idea prioritization, and align with OpenideaL’s goal of identifying successful ideas for enterprise and community stakeholders ().[](https://github.com/linnovate/openideal)

## User Stories
- **As a community member**, I want to compare two ideas side-by-side and vote for my favorite, so I can contribute to prioritizing the best ideas.
- **As a site manager**, I want users to engage in a fun, interactive voting process, so I can gather more data for the "Worth checking out" algorithm and identify high-potential ideas.
- **As an enterprise user**, I want a simple interface to collect stakeholder feedback on ideas, so I can align innovation efforts with business goals.

## Feature Details
### Functionality
- **Idea Display**: Two ideas are randomly selected from the OpenideaL database and displayed side-by-side with:
  - Idea title
  - Short description (truncated to 100 characters)
  - Current vote count and comment count
- **Voting Mechanism**: Users click a "Vote" button under their preferred idea, incrementing its vote count in the OpenideaL database.
- **Next Pair Button**: A button loads a new pair of ideas, encouraging continuous engagement.
- **Integration with Existing Features**: Votes contribute to OpenideaL’s "Worth checking out" algorithm, which weighs votes, comments, and activity recency ().[](https://www.drupal.org/project/idea)
- **Responsive Design**: The interface is mobile-friendly, aligning with OpenideaL’s fully responsive design ().[](https://www.openidealapp.com/front-page/)

### Technical Requirements
- **Front-End**: HTML/CSS/JavaScript for the user interface, styled to resemble early Facebook’s clean, blue-and-white aesthetic (circa 2008-2010).
- **Back-End Integration**: Drupal module to fetch random ideas, update vote counts, and sync with the "Worth checking out" algorithm. (Note: This proposal includes a front-end prototype; back-end implementation is for maintainers to scope.)
- **Dependencies**: Leverages Drupal’s existing APIs for idea retrieval and voting, with no new external dependencies.
- **Accessibility**: Follows OpenideaL’s commitment to accessibility, with ARIA labels and keyboard navigation ().[](https://www.openidealapp.com/front-page/)

### Wireframe
```
+-------------------+-------------------+
|     Idea 1        |     Idea 2        |
| [Title]           | [Title]           |
| [Description...]  | [Description...]  |
| Votes: [X]        | Votes: [Y]        |
| Comments: [Z]     | Comments: [W]     |
| [Vote Button]     | [Vote Button]     |
+-------------------+-------------------+
|       [Next Pair Button]            |
+-------------------+-------------------+
```

### Prototype
A standalone HTML/CSS/JavaScript prototype is included in this pull request (see `this-or-that-prototype.html`) to demonstrate the front-end design. It uses placeholder data and simulates user interaction. Maintainers can adapt this for Drupal integration.

### Benefits
- **User Engagement**: Gamifies idea prioritization, increasing community participation.
- **Enterprise Value**: Provides enterprises with a fun, data-driven way to gather stakeholder feedback, aligning with OpenideaL’s use in multi-national companies ().[](https://github.com/linnovate/openideal)
- **Strategic Alignment**: Enhances OpenideaL’s ideation workflow, supporting the platform’s goal of identifying high-potential ideas ().[](https://www.drupal.org/project/idea)

### Implementation Notes
- **Contribution Scope**: This proposal includes a specification and front-end prototype. Back-end Drupal integration (e.g., fetching ideas, updating votes) requires maintainer input.
- **Testing**: The prototype is tested in modern browsers (Chrome, Firefox, Safari). Drupal integration should include unit tests for vote updates and algorithm syncing.
- **Next Steps**: Maintainers can review the prototype, provide feedback, and assign back-end tasks if approved.

## Contribution Guidelines
- This feature is submitted as a pull request to `linnovate/openideal`, following the project’s issue queue process ().[](https://www.drupal.org/project/idea/releases)
- Feedback is welcome via the GitHub issue queue or @openideal on Twitter ().[](https://www.drupal.org/project/idea/releases)